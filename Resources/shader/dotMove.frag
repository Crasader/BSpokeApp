uniform vec2 u_texCoreCoord;        //center of mask texture coordinate
uniform vec2 u_coreCoord;           //center of gravity  0.0 ~ 1.0
uniform sampler2D u_contentTexture; //content texture
uniform vec4 u_contentTextureColor;
uniform vec2 u_ratio;               //mask.size / content.size

varying vec4 v_position;
varying vec2 v_texCoord;


void main(void)
{
    //texCoord distance to centerCoord
    vec2 coreCoord = u_coreCoord;
    coreCoord.y    = 1.0 - coreCoord.y;
    
    vec2 dis = coreCoord - v_texCoord;
    vec2 contentDis = dis * u_ratio;
    vec2 contentTexCoord = u_texCoreCoord - contentDis;
    
    vec4 maskColor = texture2D(CC_Texture0, v_texCoord);
    vec4 contentColor = u_contentTextureColor * texture2D(u_contentTexture, contentTexCoord);
    
    gl_FragColor = contentColor * maskColor.a;
}
