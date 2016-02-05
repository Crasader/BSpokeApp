//uniform vec2 textureSize; //dimensions in pixels of billboardTexture
uniform float u_ratio;    //斜率

uniform int u_clipLineCount;
uniform float u_clipY[16];
uniform float u_clipDistance[16];


varying vec4 v_position;
varying vec2 v_texCoord;


void main(void)
{
    vec4 avgColor = texture2D(CC_Texture0, v_texCoord);
    
    if(avgColor.a == 0.0){
        discard;
    }
    
    vec2 texCoord = v_texCoord;
    texCoord.y = 1.0 - texCoord.y;
    
//    float u_ratio = 0.5;
//    int u_clipLineCount = 1;
//    vec2 u_clipLineData[1];
    
//    u_clipLineData[0] = vec2(0.3, 0.2);
    
    float ratio1 = u_ratio + 0.02;
    float ratio2 = u_ratio - 0.02;
    
    float clipY1 = -ratio1 * texCoord.x + texCoord.y;
    float clipY2 = -ratio2 * texCoord.x + texCoord.y;
    float minClipY, maxClipY;
    
    for(int i = 0; i < u_clipLineCount; i++){
        minClipY = u_clipY[i];
        maxClipY = u_clipY[i] + u_clipDistance[i];
        
        if(clipY1 > minClipY && clipY1 < maxClipY && clipY2 > minClipY && clipY2 < maxClipY){
            avgColor = vec4(0.0);
            break;
        }
    }
    
    gl_FragColor = avgColor;
}
