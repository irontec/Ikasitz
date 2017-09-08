//
//  Texture2DMutable.cpp
//   Ported to C++ by Dmitry Matyukhin
//
//  CCMutableTexture.m
//	Created by Lam Hoang Pham.
//  Improved by Manuel Martinez-Almeida.
//
//  Added initWithImage(cocos2d::Image *image) by Iker Mendilibar Fernandez
//


#include "Texture2DMutable.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

//void Texture2DMutable::setTexData(void *var) {
//    data_ = var;
//}

void Texture2DMutable::releaseData(void* data)
{
	//Don't free the data
}

void* Texture2DMutable::keepData(void* data, unsigned int lenght)
{
	void *newData = malloc(lenght);
	memmove(newData, data, lenght);
	return newData;
}

bool Texture2DMutable::initWithData(const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const cocos2d::Size &contentSize)
{
	if(!Texture2D::initWithData(data, dataLen, pixelFormat, pixelsWide, pixelsHigh, contentSize)) {
        return false;
    }
        
    switch (pixelFormat) {
        case Texture2D::PixelFormat::RGBA8888:	bytesPerPixel_ = 4; break;
        case Texture2D::PixelFormat::A8:			bytesPerPixel_ = 1; break;
        case Texture2D::PixelFormat::RGBA4444:
        case Texture2D::PixelFormat::RGB565:
        case Texture2D::PixelFormat::RGB5A1:
            bytesPerPixel_ = 2;
            break;
        default:break;
    }
    data_ = (void*) data;
    dataLen_ = dataLen;
    
#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
    unsigned int max = pixelsWide*pixelsHigh*bytesPerPixel_;
    originalData_ = malloc(max);
    memcpy(originalData_, data_, max);
#endif

    return true;
}

/*
//bool Texture2DMutable::initWithImage(cocos2d::Image *image)
//{
//    data_ = (void*) image->getData();
//    if(!Texture2D::initWithImage(image))
//        return false;
//    
//    unsigned int width = image->getWidth();
//    unsigned int height = image->getWidth();
//    
//    
//    bytesPerPixel_ = 4;
//    
//#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
//    unsigned int max = width*height*bytesPerPixel_;
//    //new unsigned char[width * height * 2];
//    originalData_ = malloc(max);
//    memcpy(originalData_, data_, max);
//#endif
//    return true;
//}
*/
Color4B Texture2DMutable::pixelAt(const Point& pt)
{
	Color4B c = {0, 0, 0, 0};
	if(!data_) return c;
	if(pt.x < 0 || pt.y < 0) return c;
	if(pt.x >= _contentSize.width || pt.y >= _contentSize.height) return c;
    
	unsigned int x = pt.x, y = pt.y;
        
	if(_pixelFormat == Texture2D::PixelFormat::RGBA8888){
		unsigned int *pixel = (unsigned int *)originalData_;
		pixel = pixel + (y * _pixelsWide) + x;
		c.r = *pixel & 0xff;
		c.g = (*pixel >> 8) & 0xff;
		c.b = (*pixel >> 16) & 0xff;
		c.a = (*pixel >> 24) & 0xff;
	} else if(_pixelFormat == Texture2D::PixelFormat::RGBA4444){
		GLushort *pixel = (GLushort *)originalData_;
		pixel = pixel + (y * _pixelsWide) + x;
		c.a = ((*pixel & 0xf) << 4) | (*pixel & 0xf);
		c.b = (((*pixel >> 4) & 0xf) << 4) | ((*pixel >> 4) & 0xf);
		c.g = (((*pixel >> 8) & 0xf) << 4) | ((*pixel >> 8) & 0xf);
		c.r = (((*pixel >> 12) & 0xf) << 4) | ((*pixel >> 12) & 0xf);
	} else if(_pixelFormat == Texture2D::PixelFormat::RGB5A1){
		GLushort *pixel = (GLushort *)originalData_;
		pixel = pixel + (y * _pixelsWide) + x;
		c.r = ((*pixel >> 11) & 0x1f)<<3;
		c.g = ((*pixel >> 6) & 0x1f)<<3;
		c.b = ((*pixel >> 1) & 0x1f)<<3;
		c.a = (*pixel & 0x1)*255;
	} else if(_pixelFormat == Texture2D::PixelFormat::RGB565){
		GLushort *pixel = (GLushort *)originalData_;
		pixel = pixel + (y * _pixelsWide) + x;
		c.b = (*pixel & 0x1f)<<3;
		c.g = ((*pixel >> 5) & 0x3f)<<2;
		c.r = ((*pixel >> 11) & 0x1f)<<3;
		c.a = 255;
	} else if(_pixelFormat == Texture2D::PixelFormat::A8){
		GLubyte *pixel = (GLubyte *)originalData_;
		c.a = pixel[(y * _pixelsWide) + x];
		// Default white
		c.r = 255;
		c.g = 255;
		c.b = 255;
	}
    
	return c;
}

bool Texture2DMutable::setPixelAt(const Point& pt, Color4B c)
{
	if(!data_)return false;
	if(pt.x < 0 || pt.y < 0) return false;
	if(pt.x >= _contentSize.width || pt.y >= _contentSize.height) return false;
	unsigned int x = pt.x, y = pt.y;
    
	dirty_ = true;
    
	//	Shifted bit placement based on little-endian, let's make this more
	//	portable =/
    
	if(_pixelFormat == Texture2D::PixelFormat::RGBA8888){
		unsigned int *pixel = (unsigned int *)data_;
		pixel[(y * _pixelsWide) + x] = (c.a << 24) | (c.b << 16) | (c.g << 8) | c.r;
	} else if(_pixelFormat == Texture2D::PixelFormat::RGBA4444){
		GLushort *pixel = (GLushort *)data_;
		pixel = pixel + (y * _pixelsWide) + x;
		*pixel = ((c.r >> 4) << 12) | ((c.g >> 4) << 8) | ((c.b >> 4) << 4) | (c.a >> 4);
	} else if(_pixelFormat == Texture2D::PixelFormat::RGB5A1){
		GLushort *pixel = (GLushort *)data_;
		pixel = pixel + (y * _pixelsWide) + x;
		*pixel = ((c.r >> 3) << 11) | ((c.g >> 3) << 6) | ((c.b >> 3) << 1) | (c.a > 0);
	} else if(_pixelFormat == Texture2D::PixelFormat::RGB565){
		GLushort *pixel = (GLushort *)data_;
		pixel = pixel + (y * _pixelsWide) + x;
		*pixel = ((c.r >> 3) << 11) | ((c.g >> 2) << 5) | (c.b >> 3);
	} else if(_pixelFormat == Texture2D::PixelFormat::A8){
		GLubyte *pixel = (GLubyte *)data_;
		pixel[(y * _pixelsWide) + x] = c.a;
	} else {
		dirty_ = false;
		return false;
	}
	return true;
}

void Texture2DMutable::fill(Color4B p)
{
	for(int r = 0; r < _contentSize.height; ++r)
		for(int c = 0; c < _contentSize.width; ++c)
            this->setPixelAt(Point(c, r), p);
}

Texture2D* Texture2DMutable::copyMutable(bool isMutable)
{	
	Texture2D* co;
	if(isMutable)
	{
		int mem = _pixelsWide*_pixelsHigh*bytesPerPixel_;
		void *newData = malloc(mem);
		memcpy(newData, data_, mem);
        co = new Texture2DMutable();
        if (!co->initWithData(newData, getDataLen(), _pixelFormat, _pixelsWide, _pixelsHigh, _contentSize)) {
            delete co;
            co = NULL;
        }
	}else {
        
        co = new Texture2D();
        if (!co->initWithData(data_, getDataLen(), _pixelFormat, _pixelsWide, _pixelsHigh, _contentSize)) {
            delete co;
            co = NULL;
        }
    }
        
    return co;
}

Texture2DMutable* Texture2DMutable::copy()
{
	return (Texture2DMutable*)this->copyMutable( true );
}

void Texture2DMutable::copy(Texture2DMutable* textureToCopy, const Point& offset)
{
	for(int r = 0; r < _contentSize.height;++r){
		for(int c = 0; c < _contentSize.width; ++c){
            setPixelAt(Point(c + offset.x, r + offset.y), textureToCopy->pixelAt(Point(c, r)));
		}
	}
}

void Texture2DMutable::restore()
{
#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	memcpy(data_, originalData_, bytesPerPixel_*_pixelsWide*_pixelsHigh);
	this->apply();
#else
	//You should set CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA 1 in Texture2DMutable.h
    CCAssert(false, "Exception:  CCMutableTexture.restore was disabled by the user.");
#endif
}

void Texture2DMutable::apply()
{
	if(!data_) return;
    
	glBindTexture(GL_TEXTURE_2D, _name);
    
	switch(_pixelFormat)
	{
		case Texture2D::PixelFormat::RGBA8888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pixelsWide, _pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
			break;
		case Texture2D::PixelFormat::RGBA4444:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pixelsWide, _pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data_);
			break;
		case Texture2D::PixelFormat::RGB5A1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pixelsWide, _pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data_);
			break;
		case Texture2D::PixelFormat::RGB565:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _pixelsWide, _pixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data_);
			break;
		case Texture2D::PixelFormat::A8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, _pixelsWide, _pixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data_);
			break;
		default:
            CCAssert(false, "NSInternalInconsistencyException");

	}
	glBindTexture(GL_TEXTURE_2D, 0);
	dirty_ = false;
}

Texture2DMutable::~Texture2DMutable()
{
	CCLOGINFO("cocos2d: deallocing %p", this);
    
	//free(data_);
#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
	free(originalData_);
#endif

}

