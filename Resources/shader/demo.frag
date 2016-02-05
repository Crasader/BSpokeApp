#define KERNEL_SIZE 9
#define RANDOM_MAX_SIZE 128

uniform vec2 centerCoord;
uniform float pixelSize; //size of bigger "pixel regions". These regions are forced to be square
uniform vec2 textureSize; //dimensions in pixels of billboardTexture

varying vec4 v_position;
varying vec2 v_texCoord;

uniform float edgeRadius; //pixel

uniform int randomSizeLength;
uniform float randomSize[RANDOM_MAX_SIZE];
uniform vec2 randomSizeCoord[RANDOM_MAX_SIZE];

vec2 texCoords[KERNEL_SIZE]; //stores texture lookup offsets from a base case                   

void main(void)
{
	float usePixelSize = float(pixelSize);
    float inPixelSize  = usePixelSize;
    
    vec2 pixelCoord = gl_FragCoord.xy - (centerCoord.xy - textureSize.xy / 2.0);
    
    vec2 originCoord = floor(pixelCoord / usePixelSize) * usePixelSize;
    
    vec2 texCoordsStep = 1.0/textureSize; //width of "pixel region" in texture coords
    
    for(int i = 0; i < randomSizeLength; i++){
        if(all(greaterThanEqual(pixelCoord, randomSizeCoord[i])) && all(lessThan(pixelCoord, randomSizeCoord[i] + randomSize[i]))){
            inPixelSize = randomSize[i];
            originCoord = randomSizeCoord[i];
            break;
        }
    }
	
	vec4 avgColor; //will hold our averaged color from our sample points

    vec2 inPixelStep = 1.0/(textureSize / inPixelSize);  //width of "pixel region" divided by 3 (for KERNEL_SIZE = 9, 3x3 square)
	vec2 inPixelHalfStep = inPixelStep/2.0;
    vec2 offset = originCoord * texCoordsStep;
    
    offset = vec2(offset.x, 1.0 - offset.y); //翻转，因为texture y与 fragCoord的y方向不一样。

	//use offset (pixelBin * texCoordsStep) from base case (the lower left corner of billboard) to compute texCoords
	texCoords[0] = vec2(inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + offset;
	texCoords[1] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + offset;
	texCoords[2] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + offset;
	texCoords[3] = vec2(inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + offset;
	texCoords[4] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + offset;
	texCoords[5] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + offset;
	texCoords[6] = vec2(inPixelHalfStep.x, inPixelHalfStep.y) + offset;
	texCoords[7] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelHalfStep.y) + offset;
	texCoords[8] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelHalfStep.y) + offset;
		
	//take average of 9 pixel samples
	avgColor = texture2D(CC_Texture0, texCoords[0]) +
				texture2D(CC_Texture0, texCoords[1]) +
				texture2D(CC_Texture0, texCoords[2]) +
				texture2D(CC_Texture0, texCoords[3]) +
				texture2D(CC_Texture0, texCoords[4]) +
				texture2D(CC_Texture0, texCoords[5]) +
				texture2D(CC_Texture0, texCoords[6]) +
				texture2D(CC_Texture0, texCoords[7]) +
				texture2D(CC_Texture0, texCoords[8]);
		
	avgColor /= float(KERNEL_SIZE);

    //add edge
    if(any(lessThanEqual(pixelCoord, originCoord + edgeRadius)) || any(greaterThan(pixelCoord, originCoord + inPixelSize - edgeRadius))){
        avgColor.a = 0.0;
    }
    
    gl_FragColor = avgColor;
/**
    float a,b,c;
    a = 2.0;
    c = mod(float(gl_PointCoord.x * float(textureSize.x)), a);

    if(usePixelSize == 16.0){
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
 **/
}
