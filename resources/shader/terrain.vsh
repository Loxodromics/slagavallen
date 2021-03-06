uniform highp mat4 qt_Matrix;
attribute highp vec4 qt_VertexPosition;
attribute highp vec2 qt_VertexTexCoord;
attribute highp vec3 vertexNormal;
varying highp vec2 qt_TexCoord;
varying highp vec3 normal;
varying highp vec3 fragPos;

void main() {
	qt_TexCoord = qt_VertexTexCoord;
	gl_Position = qt_Matrix * qt_VertexPosition;

	fragPos = qt_VertexPosition.xyz;
	normal = vertexNormal;
}
