#define LINE_COUNT_MAX 12

uniform float u_basePointPos[LINE_COUNT_MAX];
uniform float u_offset[LINE_COUNT_MAX];
uniform int u_lineCount;
uniform float u_angle;
uniform int u_isXSpread;

varying vec2 v_texCoord;
varying vec4 v_fragmentColor;


float calculateY(float x0, float y0, float k){
	return y0 - k * x0;
}

vec2 transfer(vec2 texCoord, float angle){
	texCoord.t = 1.0 - texCoord.t;

	vec2 ret = texCoord;
    
	float k  = tan(angle);
	float y = calculateY(texCoord.s, texCoord.t, k);
    
    for(int i = 0; i < u_lineCount; i++){
        float basePointPos = u_basePointPos[i];
        float basePointY;
        
        if(i == u_lineCount - 1){
            vec2 offsetVec = vec2(cos(angle), sin(angle)) * u_offset[i];
            ret += offsetVec;
            break;
        }
        
        if(u_isXSpread == 1)
            basePointY = calculateY(basePointPos, 0.5, k);
        else
            basePointY = calculateY(0.5, basePointPos, k);
        
        if(y > basePointY){
            vec2 offsetVec = vec2(cos(angle), sin(angle)) * u_offset[i];
            ret += offsetVec;
            break;
        }
    }

	ret.t = 1.0 - ret.t;

	return ret;
}

void main()
{
	int lineCount = u_lineCount;
	
    if(lineCount == 0){
        gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, v_texCoord);
        return;
    }
        
    float angle = u_angle;
    vec2 originTexCoord = v_texCoord;
	vec2 changedTexCoord = transfer(originTexCoord, angle);
    
	gl_FragColor = v_fragmentColor * texture2D(CC_Texture0, changedTexCoord);
}