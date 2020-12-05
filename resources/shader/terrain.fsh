uniform highp sampler2D source;
uniform highp float qt_Opacity;
varying highp vec2 qt_TexCoord;
varying highp vec3 normal;
varying highp vec3 fragPos;

void main() {

	vec3 lightPos = vec3(-50000.0, 50000.0, -100000.0);
	vec3 lightDir = normalize(lightPos - fragPos);
	highp float diff = 2.0 * max(dot(normal, lightDir), 0.0) - 0.5;

	highp vec4 p = texture2D(source, qt_TexCoord);
	highp float a = qt_Opacity * p.a;
	gl_FragColor = vec4(p.rgb * diff, a);


}
