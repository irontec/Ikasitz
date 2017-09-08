//
//  Texture2DMutable.h
//   Ported to C++ by Dmitry Matyukhin
//
//  Texture2DMutable.m
//	Created by Lam Hoang Pham.
//  Improved by Manuel Martinez-Almeida.
//

#ifndef Texture2DMutable_cpp
#define Texture2DMutable_cpp

#include "cocos2d.h"

#define CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA 1

using namespace cocos2d;

class CC_DLL Texture2DMutable : public Texture2D
{

private:
#if CC_MUTABLE_TEXTURE_SAVE_ORIGINAL_DATA
    CC_SYNTHESIZE(void*, originalData_, OriginalTexData);
#endif
    CC_SYNTHESIZE(void*, data_, TexData);
    CC_SYNTHESIZE(ssize_t, dataLen_, DataLen);

    
	unsigned int bytesPerPixel_;
	bool dirty_;

public:
    ~Texture2DMutable();
    
	// These functions are needed to create mutable textures
	void releaseData(void *data);
	void* keepData(void *data, unsigned int length);
    
	// Intializes with a texture2d with data
	bool initWithData(const void *data, ssize_t dataLen, Texture2D::PixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const cocos2d::Size &contentSize);
    
    //bool initWithImage(Image *image);
    Color4B pixelAt(const Point& pt);
    
    ///
    //	@param pt is a point to get a pixel (0,0) is top-left to (width,height) bottom-right
    //	@param c is a Color4B which is a colour.
    //	@returns if a pixel was set
    //	Remember to call apply to actually update the texture canvas.
    ///
    bool setPixelAt(const Point& pt, Color4B c);
    
    ///
    //	Fill with specified colour
    ///
    void fill(Color4B c);
    
    ///
    //	@param textureToCopy is the texture image to copy over
    //	@param offset also offset's the texture image
    ///
    Texture2D* copyMutable(bool isMutable);
    
    Texture2DMutable* copy();
    
    void copy(Texture2DMutable* textureToCopy, const Point& offset);
    
    void restore();
    ///
    //	apply actually updates the texture with any new data we added.
    ///
    void apply();
};

#endif // Texture2DMutable_cpp
