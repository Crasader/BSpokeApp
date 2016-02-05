uniform sampler2D u_contentTexture;     //content texture
uniform vec4 u_contentTextureColor;
uniform float u_contentTextureOffset;   //content texture offset(distance), used for animation
uniform vec2 u_baseOffset;              //base texture offset;
uniform vec2 u_ratio;                   //mask.size / content.size
uniform float u_angle;                  //rotate angle
varying vec4 v_position;
varying vec2 v_texCoord;


void main(void)
{
    float cosAngle = cos(u_angle);
    float sinAngle = sin(u_angle);
    
    vec2 baseOffset = u_baseOffset;
    vec2 offset = vec2(u_contentTextureOffset * sinAngle, u_contentTextureOffset * cosAngle);
    
    //rotate content texture
    vec2 contentTexCoord = offset + baseOffset + v_texCoord * u_ratio;
    contentTexCoord -= vec2(0.5, 0.5);
    
    vec2 afterRotateCoord = vec2(contentTexCoord.x * cosAngle - contentTexCoord.y * sinAngle, contentTexCoord.x * sinAngle + contentTexCoord.y * cosAngle);
    afterRotateCoord += vec2(0.5, 0.5);
    
    vec4 maskColor = texture2D(CC_Texture0, v_texCoord);
    vec4 contentColor = u_contentTextureColor * texture2D(u_contentTexture, afterRotateCoord);
    
    if(afterRotateCoord.x > 1.0 || afterRotateCoord.x < 0.0){
        contentColor.rgb = vec3(1.0, 0.0, 0.0);
    }
    
    gl_FragColor = contentColor * maskColor.a;
}
