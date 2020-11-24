uniform sampler2D texture;
uniform lowp float qt_Opacity;
uniform lowp vec4 sunPosition;

varying highp vec2 vTexCoord;

void main()
{
	lowp vec4 c = texture2D(texture, vTexCoord);
	gl_FragColor = c * qt_Opacity;
}
