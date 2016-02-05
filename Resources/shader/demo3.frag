#define KERNEL_SIZE 9

uniform float pixelSize; //size of bigger "pixel regions". These regions are forced to be square
uniform vec2 textureSize; //dimensions in pixels of CC_Texture0
uniform float pixelRadius;
uniform vec2 u_coreCoord;  //center of gravity  0.0 ~ 1.0

varying vec4 v_position;
varying vec2 v_texCoord;


vec2 texCoords[KERNEL_SIZE]; //stores texture lookup offsets from a base case                   

void main(void)
{
    vec4 avgColor; //will hold our averaged color from our sample points
    avgColor = texture2D(CC_Texture0, v_texCoord);
    
    if(avgColor.a == 0.0){
        gl_FragColor = avgColor;
        discard;
    }
    
    vec2 coreCoord       = vec2(u_coreCoord.x, 1.0 - u_coreCoord.y);
    vec2 texCoordsStep = 1.0 / (textureSize / pixelSize); //width of "pixel region" in texture coords
    vec2 pixelRegionCoords = fract(v_texCoord.st/texCoordsStep); //x and y coordinates within "pixel region"
    vec2 pixelBin = floor(v_texCoord.st/texCoordsStep); //"pixel region" number counting away from base case
    vec2 inPixelStep = texCoordsStep/3.0; //width of "pixel region" divided by 3 (for KERNEL_SIZE = 9, 3x3 square)
    vec2 inPixelHalfStep = inPixelStep/2.0;
    vec2 texOffset       = pixelBin * texCoordsStep;
    
    float radius = 0.2;
    
    if(distance(texOffset, vec2(0.5, 0.5)) < radius){
        float maxDistance = distance(coreCoord, vec2(0.5, 0.5)) + radius;  //重心和(0.5, 0.5)连线到圆的最大值
        float toCoreDistance = distance(texOffset, coreCoord);           //像素点到重心距离
        
        texOffset = vec2(0.5, 0.5) + (toCoreDistance / maxDistance) * radius;
    }
    
    //use offset (pixelBin * texCoordsStep) from base case (the lower left corner of billboard) to compute texCoords
    texCoords[0] = vec2(inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + texOffset;
    texCoords[1] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + texOffset;
    texCoords[2] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y*2.0 + inPixelHalfStep.y) + texOffset;
    texCoords[3] = vec2(inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + texOffset;
    texCoords[4] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + texOffset;
    texCoords[5] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelStep.y + inPixelHalfStep.y) + texOffset;
    texCoords[6] = vec2(inPixelHalfStep.x, inPixelHalfStep.y) + texOffset;
    texCoords[7] = vec2(inPixelStep.x + inPixelHalfStep.x, inPixelHalfStep.y) + texOffset;
    texCoords[8] = vec2(inPixelStep.x*2.0 + inPixelHalfStep.x, inPixelHalfStep.y) + texOffset;
    
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
    
    //blend between fragments in the circle and out of the circle defining our "pixel region"
    //Equation of a circle: (x - h)^2 + (y - k)^2 = r^2
    vec2 powers = pow(abs(pixelRegionCoords - 0.5),vec2(2.0));
    float radiusSqrd = pow(pixelRadius, 2.0);
    
    if(powers.x+powers.y > radiusSqrd){
        avgColor = vec4(0.0);
    }
    gl_FragColor = avgColor;
}
