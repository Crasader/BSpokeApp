varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main(void)
{
    vec4 textureColor = texture2D(CC_Texture0, v_texCoord);
    gl_FragColor = v_fragmentColor * textureColor.a;
    gl_FragColor.a = textureColor.a;
}
