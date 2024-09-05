#include "Angel.h"  
#include<math.h>
using namespace std;

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );

// Default specifications
const int TOTAL_CUBES = 16;
const int POINTS_PER_CUBE = 36;
//const int TOTAL_CUBES_POINTS = TOTAL_CUBES * POINTS_PER_CUBE;
const int TOTAL_CUBES_POINTS = 5000;

const int TOTAL_COLORS = 16;
typedef vec4 point4;
typedef vec4 color4;
point4 points[TOTAL_CUBES_POINTS];
color4 colors[TOTAL_CUBES_POINTS];
vec3 normals[TOTAL_CUBES_POINTS]; // reflection
point4 vertices[1000];
color4 vertex_colors[TOTAL_COLORS]; // unnecessary 
GLuint program;

// Global variable
int vertexDataCount = 0;
GLfloat BASE_WIDTH = 1.0;

// Color 
color4 RED = color4(1.0, 0.0, 0.0, BASE_WIDTH); // 0
//color4 YELLOW = color4(1.0, 1.0, 0.0, BASE_WIDTH); // 1
//color4 GREEN = color4(0.0, 1.0, 0.0, BASE_WIDTH); // 2
//color4 BLUE = color4(0.0, 0.0, 1.0, BASE_WIDTH); // 3
//color4 MAGENTA = color4(1.0, 0.0, 1.0, BASE_WIDTH); // 4
//color4 WHITE = color4(1.0, 1.0, 1.0, BASE_WIDTH); // 5
//color4 CYAN = color4(0.0, 1.0, 1.0, BASE_WIDTH); // 6
//color4 BROWN = color4(0.4, 0.0, 0.0, BASE_WIDTH); // 7
//color4 LEMON_YELLOW = color4(1.0, 0.8, 0.0, BASE_WIDTH); // 8
//color4 LIGHT_GREEN = color4(0.5, 0.8, 0.0, BASE_WIDTH); // 9
//color4 BEIGE = color4(1.0, 1.0, 0.9, BASE_WIDTH); // 10
//color4 LIGHT_PINK = color4(1.0, 0.89, 0.83, BASE_WIDTH); // 11
//color4 ORANGE = color4(1.0, 0.6, 0.2, BASE_WIDTH); // 12
//color4 LIGHT_BROWN = color4(0.5, 0.29, 0.0, BASE_WIDTH); // 13
//color4 BLUE_SKY = color4(0.0, 1.0, 1.0, BASE_WIDTH); // 14
//color4 SLATE_GRAY = color4(0.725, 0.827, 0.93, BASE_WIDTH); // 15

// model
GLuint model;
mat4 ctm;

// camera
GLuint camera;
mat4 cameraCtm;
GLfloat camXLoca = 0.0, camYLoca = 0.0, camZLoca = 5.5; 
GLfloat camXTran = 0.0, camYTran = 0.0, camZTran = 0.0,
camXRot = 0.0, camYRot = 0.0, camZRot = 0.0;
mat4 camChangingCtm;
vec4 eye = vec4(camXLoca, camYLoca, camZLoca, 1.0), at = vec4(0.0, 0.0, 0.0, 1.0), up = vec4(0.0, 1.0, 0.0, 1.0);

// projector
GLuint projector;
mat4 projectorCtm;
GLfloat leftLim = -5.0, rightLim = 5.0, botLim = -5.0, topLim = 5.0, nearLim = 3.5, farLim = 100.0;

void calculateCubeCoordinator() {
	// basic cube coordinator
	vertices[0] = point4(-0.5, -0.5, 0.5, BASE_WIDTH);
	 vertices[1] = point4(-0.5, 0.5, 0.5, BASE_WIDTH);
	 vertices[2] = point4(0.5, 0.5, 0.5, BASE_WIDTH);
	 vertices[3] = point4(0.5, -0.5, 0.5, BASE_WIDTH);
	 vertices[4] = point4(-0.5, -0.5, -0.5, BASE_WIDTH);
	 vertices[5] = point4(-0.5, 0.5, -0.5, BASE_WIDTH);
	 vertices[6] = point4(0.5, 0.5, -0.5, BASE_WIDTH);
	 vertices[7] = point4(0.5, -0.5, -0.5, BASE_WIDTH);
	 vertices[8] = point4(0.0, 0.5, 0.0, BASE_WIDTH);

	 // rocket
	 vertices[9] = point4(0.0, 0.5, 0.0, BASE_WIDTH);
	 vertices[10] = point4(0.4, 0.5, 0.0, BASE_WIDTH);
	 vertices[11] = point4(0.5, 0.0, 0.0, BASE_WIDTH);
	 vertices[12] = point4(0.0, 0.0, 0.5, BASE_WIDTH);
	 vertices[13] = point4(0.0, 0.0, -0.5, BASE_WIDTH);

	// // balloon
	 GLfloat val1 = 1.0, val2 = sqrt(3) / 2, val3 = sqrt(2) / 2, val4 = 0.5, val5 = 0;
	 vertices[14] = point4(0.0, 1.0, 0.2, BASE_WIDTH); 
	 vertices[15] = point4(0.1736, 0.9848, 0.2, BASE_WIDTH); 
	 vertices[16] = point4(0.342, 0.9396, 0.2, BASE_WIDTH); 
	 vertices[17] = point4(0.5, val2, 0.2, BASE_WIDTH); 
	 vertices[18] = point4(0.6427, 0.766, 0.2, BASE_WIDTH); 
	 vertices[19] = point4(0.766, 0.6427, 0.2, BASE_WIDTH); 
	 vertices[20] = point4(val2, 0.5, 0.2, BASE_WIDTH); 
	 vertices[21] = point4(0.9396, 0.342, 0.2, BASE_WIDTH); 
	 vertices[22] = point4(0.984, 0.1736, 0.2, BASE_WIDTH); 
	 vertices[23] = point4(1.0, 0.0, 0.2, BASE_WIDTH); 
	 vertices[24] = point4(0.0, 1.0, -0.2, BASE_WIDTH); 
	 vertices[25] = point4(0.1736, 0.9848, -0.2, BASE_WIDTH); 
	 vertices[26] = point4(0.342, 0.9396, -0.2, BASE_WIDTH); 
	 vertices[27] = point4(0.5, val2, -0.2, BASE_WIDTH); 
	 vertices[28] = point4(0.6427, 0.766, -0.2, BASE_WIDTH); 
	 vertices[29] = point4(0.766, 0.6427, -0.2, BASE_WIDTH); 
	 vertices[30] = point4(val2, 0.5, -0.2, BASE_WIDTH); 
	 vertices[31] = point4(0.9396, 0.342, -0.2, BASE_WIDTH); 
	 vertices[32] = point4(0.984, 0.1736, -0.2, BASE_WIDTH); 
	 vertices[33] = point4(1.0, 0.0, -0.2, BASE_WIDTH);
	 vertices[34] = point4(0.0, 0.0, 0.2, BASE_WIDTH);
	 vertices[35] = point4(0.0, 0.0, -0.2, BASE_WIDTH);

	// RobotBody
	 vertices[36] = point4(-0.5, 0.5, 0.5, BASE_WIDTH); 
	 vertices[37] = point4(0.5, 0.5, 0.5, BASE_WIDTH); 
	 vertices[38] = point4(0.5, 0.5, -0.5, BASE_WIDTH); 
	 vertices[39] = point4(-0.5, 0.5, -0.5, BASE_WIDTH); 

	 vertices[40] = point4(-0.35, -0.5, -0.35, BASE_WIDTH);
	 vertices[41] = point4(0.35, -0.5, -0.35, BASE_WIDTH); 
	 vertices[42] = point4(0.35, -0.5, 0.2, BASE_WIDTH); 
	 vertices[43] = point4(-0.35, -0.5, 0.2, BASE_WIDTH); 

	 vertices[44] = point4(-0.35, -0.1, 0.5, BASE_WIDTH); 
	 vertices[45] = point4(0.35, -0.1, 0.5, BASE_WIDTH); 

	 // Lower Shoes sole
	 vertices[46] = point4(-0.5, 0.5, 0.5, BASE_WIDTH);
	 vertices[47] = point4(0.5, 0.5, 0.5, BASE_WIDTH); 
	 vertices[48] = point4(0.5, 0.5, -0.5, BASE_WIDTH); 
	 vertices[49] = point4(-0.5, 0.5, -0.5, BASE_WIDTH); 
	 vertices[50] = point4(-0.35, -0.5, -0.5, BASE_WIDTH); 
	 vertices[51] = point4(0.35, -0.5, -0.5, BASE_WIDTH); 
	 vertices[52] = point4(0.35, -0.5, 0.5, BASE_WIDTH);
	 vertices[53] = point4(-0.35, -0.5, 0.5, BASE_WIDTH);

	 //Shoes Body
	 vertices[54] = point4(-0.5, 0.5, 0.5, BASE_WIDTH); 
	 vertices[55] = point4(0.5, 0.5, 0.5, BASE_WIDTH); 
	 vertices[56] = point4(0.5, 0.5, -0.5, BASE_WIDTH); 
	 vertices[57] = point4(-0.5, 0.5, -0.5, BASE_WIDTH); 
	 vertices[58] = point4(-0.35, -0.5, -0.5, BASE_WIDTH); 
	 vertices[59] = point4(0.35, -0.5, -0.5, BASE_WIDTH); 
	 vertices[60] = point4(0.35, -0.5, -0.1, BASE_WIDTH); 
	 vertices[61] = point4(-0.35, -0.5, -0.1, BASE_WIDTH);
}

void triangle(int a, int b, int c, color4 color) {
	// reflection
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[a]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[b]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[c]; vertexDataCount++;
}

void quad(int a, int b, int c, int d, color4 color)
{
	// reflection
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[a]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[b]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[c]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[a]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[c]; vertexDataCount++;
	normals[vertexDataCount] = normal; colors[vertexDataCount] = color; points[vertexDataCount] = vertices[d]; vertexDataCount++;
}

void generateColoredCube(color4 color)
{
	quad(1, 0, 3, 2, color);
	quad(2, 3, 7, 6, color);
	quad(3, 0, 4, 7, color);
	quad(6, 5, 1, 2, color);
	quad(4, 5, 6, 7, color);
	quad(5, 4, 0, 1, color);
}

void genRocketComponents() {
	// upper (18 diem) - Offset = 576
	triangle(8, 0, 3, RED);
	triangle(8, 3, 7, RED);
	triangle(8, 7, 4, RED);
	triangle(8, 4, 0, RED);
	quad(0, 3, 7, 4, RED);
	
	// mid body (18) - Offset = 594
	triangle(9, 10, 12, RED); 
	triangle(12, 10, 11, RED); 
	triangle(11, 10, 13, RED); 
	triangle(13, 10, 9, RED); 
	triangle(9, 12, 13, RED); 
	triangle(13, 11, 12, RED); 
}

void genOneQuarterBalloon() {
	/*mat tren + duoi offset : 612 - count : 108*/
	triangle(34, 14, 15, RED);
	triangle(34, 15, 16, RED);
	triangle(34, 16, 17, RED);
	triangle(34, 17, 18, RED);
	triangle(34, 18, 19, RED);
	triangle(34, 19, 20, RED);
	triangle(34, 20, 21, RED);
	triangle(34, 21, 22, RED);
	triangle(34, 22, 23, RED);
	// mat duoi (24 - 33) 
	triangle(35, 24, 25, RED);
	triangle(35, 25, 26, RED);
	triangle(35, 26, 27, RED);
	triangle(35, 27, 28, RED);
	triangle(35, 28, 29, RED);
	triangle(35, 29, 30, RED);
	triangle(35, 30, 31, RED);
	triangle(35, 31, 32, RED);
	triangle(35, 32, 33, RED);
	// mat trong ngoai
	quad(14, 15, 25, 24, RED);
	quad(15, 16, 26, 25, RED);
	quad(16, 17, 27, 26, RED);
	quad(17, 18, 28, 27, RED);
	quad(18, 19, 29, 28, RED);
	quad(19, 20, 30, 29, RED);
	quad(20, 21, 31, 30, RED);
	quad(21, 22, 32, 31, RED);
	quad(22, 23, 33, 32, RED);
}

void genRobotBody() {
	// Offset : 720 - Count : 48
	// mat tren 
	quad(36, 37, 38, 39 , RED); // 6
	// mat sau 
	quad(38, 39, 40, 41, RED); // 6
	// mat phai 
	triangle(38, 45, 37 , RED); // 3
	quad(38, 41, 42, 45 , RED); // 6
	// mat truoc
	quad(36, 37, 45, 44 , RED); // 6
	quad(44, 45, 42, 43 , RED); // 6
	// mat trai
	triangle(36, 44, 39 , RED); // 3
	quad(43, 40, 39, 36 , RED); // 6
	// mat duoi 
	quad(40, 41, 42, 43 , RED); // 6
}

void genShoes() {
	// lower : 768 - Count : 36
	quad(46, 47, 48, 49, RED); // tren
	quad(49, 48, 51, 50, RED); // sau
	quad(50, 49, 46, 53, RED); // trai
	quad(53, 46, 47, 52, RED); // truoc
	quad(52, 47, 48, 51, RED); // phai
	quad(51, 50, 53, 52, RED); // duoi

	// body : 804 - Count : 36
	quad(54, 55, 56, 57, RED); // tren
	quad(57, 56, 59, 58, RED); // sau
	quad(58, 57, 54, 61, RED); // trai
	quad(61, 54, 55, 60, RED); // truoc
	quad(60, 55, 56, 59, RED); // phai
	quad(59, 58, 61, 60, RED); // duoi
}

void generateCubes() {
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
	generateColoredCube(RED);
}

void generateGeometry()
{
	calculateCubeCoordinator();
	generateCubes();
	genRocketComponents();
	genOneQuarterBalloon();
	genRobotBody();
	genShoes();
}

void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	// reflection
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors); 
	// reflection
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);
}

// initialize reflection 
void initReflection(GLfloat r, GLfloat g, GLfloat b, GLfloat rDif, GLfloat gDif, GLfloat bDif, GLfloat rSpe, GLfloat gSpe, GLfloat bSpe) {
	point4 light_position(0.0, 36.0, 0.0, 0.0);
	color4 light_ambient(1.0, 1.0, 1.0, 1.0);
	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 light_specular(1.0, 1.0, 1.0, 1.0);

	color4 material_ambient(r, g, b, 1.0);
	color4 material_diffuse(rDif, gDif, bDif, 1.0);
	color4 material_specular(rSpe, gSpe, bSpe, 1.0);
	float material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
}


void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" ); 
    glUseProgram(program);

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

	model = glGetUniformLocation(program, "model");
	camera = glGetUniformLocation(program, "camera");
	projector = glGetUniformLocation(program, "projector");
	
	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        
}

// Outter Structure Objects
class StoreLayout {
public:
	GLfloat /*Horzontal : Left and Right Wall; Vertical : Bottom an Top Wall; Face : Front and Back Wall*/
		// Scale
		X_SCALE_HORIZONTAL = 0.96, Y_SCALE_HORIZONTAL = 72.0, Z_SCALE_HORIZONTAL = 96.0,
		X_SCALE_VERTICAL = 72.0, Y_SCALE_VERTICAL = 0.96, Z_SCALE_VERTICAL = 96.0,
		X_SCALE_FACE = 72.0, Y_SCALE_FACE = 72.0, Z_SCALE_FACE = 0.96,
		// Tran 
		X_LEFT_TRAN = -35.52, X_RIGHT_TRAN = 35.52,
		Y_BOT_TRAN = -35.52, Y_TOP_TRAN = 35.52,
		Z_FRONT_TRAN = 47.52, Z_BACK_TRAN = -47.52;

	StoreLayout() {

	}

	void drawOutlined() {
		/*set up color with Light Reflection*/
		//  Front Wall
		initReflection(0.8039216, 0.7764706, 0.4509804, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9); // khaki3
		glUniformMatrix4fv(model, 1, GL_TRUE, Translate(0.0, 0.0, Z_FRONT_TRAN) * Scale(X_SCALE_FACE, Y_SCALE_FACE, Z_SCALE_FACE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		//  Back Wall
		initReflection(0.8039216, 0.7764706, 0.4509804, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9); // khaki3
		glUniformMatrix4fv(model, 1, GL_TRUE, Translate(0.0, 0.0, Z_BACK_TRAN) * Scale(X_SCALE_FACE, Y_SCALE_FACE, Z_SCALE_FACE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		//  Left Wall
		initReflection(0.8039216, 0.7764706, 0.4509804, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9); // khaki3
		glUniformMatrix4fv(model, 1, GL_TRUE, Translate(X_LEFT_TRAN, 0.0, 0.0) * Scale(X_SCALE_HORIZONTAL, Y_SCALE_HORIZONTAL, Z_SCALE_HORIZONTAL));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		//  Right Wall
		initReflection(0.8039216, 0.7764706, 0.4509804, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9); // khaki3
		glUniformMatrix4fv(model, 1, GL_TRUE, Translate(X_RIGHT_TRAN, 0.0, 0.0) * Scale(X_SCALE_HORIZONTAL, Y_SCALE_HORIZONTAL, Z_SCALE_HORIZONTAL));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		//  Bottom Wall
		initReflection(0.33333334, 0.101960786, 0.54509807, 0.4, 0.4, 0.4, 0.5, 0.5, 0.5); // purple5
		glUniformMatrix4fv(model, 1, GL_TRUE, Translate(0.0, Y_BOT_TRAN, 0.0) * Scale(X_SCALE_VERTICAL, Y_SCALE_VERTICAL, Z_SCALE_VERTICAL));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		//  Top Wall
		initReflection(0.8039216, 0.7764706, 0.4509804, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9); // khaki3
		glUniformMatrix4fv(model, 1, GL_TRUE, Translate(0.0, Y_TOP_TRAN, 0.0) * Scale(X_SCALE_VERTICAL, Y_SCALE_VERTICAL, Z_SCALE_VERTICAL));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	}

	void drawObject() {
		drawOutlined();
	}
};

// Inner Structure Objects
class Shelf {
	GLfloat BACK_X_SCALE = 20, BACK_Y_SCALE = 10, BACK_Z_SCALE = 0.5,
		EDGE_X_SCALE = 0.5, EDGE_Y_SCALE = 10, EDGE_Z_SCALE = 4.0,
		STACK_X_SCALE = 20, STACK_Y_SCALE = 0.5, STACK_Z_SCALE = 4.0,
		LEFT_X_TRAN = -9.75, LEFT_Z_TRAN = 2.25;
	mat4 tranShelfCtm;
public:
	Shelf(mat4 tranShelfCtm) {
		this->tranShelfCtm = tranShelfCtm;
	}

	void drawObject() {
		// back SaddleBrown	(139 69 19)
		initReflection(0.54509807, 0.27058825, 0.07450981, 0.4, 0.4, 0.4, 0.64509807, 0.37058825, 0.17450981);
		glUniformMatrix4fv(model, 1, GL_TRUE, tranShelfCtm * Scale(BACK_X_SCALE, BACK_Y_SCALE, BACK_Z_SCALE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		// left Peru (205 133 63)
		initReflection(0.8039216, 0.52156866, 0.24705882, 0.4, 0.4, 0.4, 1.0, 0.72156866, 0.5705882);
		glUniformMatrix4fv(model, 1, GL_TRUE, tranShelfCtm * Translate(LEFT_X_TRAN, 0.0, LEFT_Z_TRAN) * Scale(EDGE_X_SCALE, EDGE_Y_SCALE, EDGE_Z_SCALE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		// right
		glUniformMatrix4fv(model, 1, GL_TRUE, tranShelfCtm * Translate(-LEFT_X_TRAN, 0.0, LEFT_Z_TRAN) * Scale(EDGE_X_SCALE, EDGE_Y_SCALE, EDGE_Z_SCALE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		// upper stack
		glUniformMatrix4fv(model, 1, GL_TRUE, tranShelfCtm * Translate(0.0, 4.75, 2.25) * Scale(STACK_X_SCALE, STACK_Y_SCALE, STACK_Z_SCALE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		// lower stack
		glUniformMatrix4fv(model, 1, GL_TRUE, tranShelfCtm * Translate(0.0, -4.75, 2.25) * Scale(STACK_X_SCALE, STACK_Y_SCALE, STACK_Z_SCALE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
		// mid stack
		glUniformMatrix4fv(model, 1, GL_TRUE, tranShelfCtm * Translate(0.0, 0.0, 2.25) * Scale(STACK_X_SCALE, STACK_Y_SCALE, STACK_Z_SCALE));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	}
};

GLfloat wheelRotZ = 0.0;
void drawCabin(mat4 carCtm) {
	// upper
	initReflection(0.0, 0.54509807, 0.27058825, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9); // SpringGreen
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, 3.0, 0.0) * Scale(4.0, 0.1, 4.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// right sheet
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, 0.45, 1.95) * Scale(4.0, 5.0, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// left sheet
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, 0.45, -1.95) * Scale(4.0, 5.0, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// bottom sheet
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-5.0, -2.0, 0.0) * Scale(14.0, 0.1, 4.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// front sheet
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(1.95, 0.45, 0.0) * Scale(0.1, 5.0, 4.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// back sheet
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-1.95, 0.45, 0.0) * Scale(0.1, 5.0, 4.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// right glass  
	initReflection(0.9411765, 1.0, 1.0, 0.2, 0.2, 0.2, 1.0, 1.0, 1.0); // Azure1
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.5, 1.55, 1.96) * Scale(2.0, 1.5, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// left glass 
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.5, 1.55, -1.96) * Scale(2.0, 1.5, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// front glass 
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(1.96, 1.7, 0.0) * Scale(0.1, 1.8, 3.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// Front decor highlight
	initReflection(0.0, 0.0, 0.0, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8); // Black
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.0, -1.8, 0.0) * Scale(0.3, 0.5, 4.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// Front decor Id board
	initReflection(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0); // White
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, -1.8, 0.0) * Scale(0.3, 0.5, 1.0));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// Front decor lights
	for (int i = 0; i < 360; i++) {
		initReflection(1.0, 0.75686276, 0.14509805, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // Goldenrod1
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(1.96, -1.0, 1.3) * RotateY(90) * RotateZ(i * 1.0) * Scale(0.4, 0.4, 0.1));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	}
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(1.96, -1.0, -1.3) * RotateY(90) * RotateZ(i * 1.0) * Scale(0.4, 0.4, 0.1));
		glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	}
	// fornt decor Radiators
	initReflection(0.0, 0.0, 0.0, 0.4, 0.4, 0.4, 0.8, 0.8, 0.8); // Black
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, 0.3, 0.0) * Scale(0.1, 0.05, 2.3));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, 0.15, 0.0) * Scale(0.1, 0.05, 2.3));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, 0.0, 0.0) * Scale(0.1, 0.05, 2.3));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, -0.15, 0.0) * Scale(0.1, 0.05, 2.3));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, -0.3, 0.0) * Scale(0.1, 0.05, 2.3));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(2.01, -0.45, 0.0) * Scale(0.1, 0.05, 2.3));
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}

GLfloat liftTrunkRotateZ = 0.0;
GLfloat topTrunkSheetXTran = 0.0, topTrunkSheetYTran = 0.0;
void drawTrunk(mat4 carCtm, int mode) {
	GLfloat factor = (mode == 1) ? 1.0 : 0.0;
	// lower 
	initReflection(0.93333334, 0.2509804, 0.0, 0.3, 0.0, 0.0, 1.0, 1.0, 1.0); // OrangeRed2	(238 64 0)
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-7.25 - 4.75, -1.8 - 0.15, 0.0) * RotateZ(liftTrunkRotateZ * factor) * Translate(4.75, 0.15, 0.0) * Scale(9.5, 0.3, 4.0));
	glDrawArrays(GL_TRIANGLES, 36 * 3, POINTS_PER_CUBE);
	// right 
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-7.25 - 4.75, -0.3 - 1.5, 1.85) * RotateZ(liftTrunkRotateZ * factor) * Translate(4.75, 1.5, 0.0) * Scale(9.5, 3.0, 0.3));
	glDrawArrays(GL_TRIANGLES, 36 * 2, POINTS_PER_CUBE);
	// left 
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-7.25 - 4.75, -0.3 - 1.5, -1.85) * RotateZ(liftTrunkRotateZ * factor) * Translate(4.75, 1.5, 0.0) * Scale(9.5, 3.0, 0.3));
	glDrawArrays(GL_TRIANGLES, 36 * 2, POINTS_PER_CUBE);
	// top
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-2.65 - 0.15 - topTrunkSheetXTran * factor, -0.3 - 1.5 + topTrunkSheetYTran * factor, 0.0) * RotateZ(liftTrunkRotateZ * factor) * Translate(0.15, 1.5, 0.0) * Scale(0.3, 3.0, 4.0));
	glDrawArrays(GL_TRIANGLES, 36 * 1, POINTS_PER_CUBE); // tam nay
	// bot
	glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-11.85 - 0.15, -0.3 - 1.5, 0.0) * RotateZ(liftTrunkRotateZ * factor) * Translate(0.15, 1.5, 0.0) * Scale(0.3, 3.0, 4.0));
	glDrawArrays(GL_TRIANGLES, 36 * 1, POINTS_PER_CUBE);
}

void drawWheel(mat4 carCtm, int mode) {
	// front wheel
	GLfloat factor = (mode == 1) ? 1.0 : 0.0;
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // black
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, -2.0, 1.61) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(1.0, 1.7, 0.8)); // 0.35
		glDrawArrays(GL_TRIANGLES, 36 * 7, POINTS_PER_CUBE);
	}
	initReflection(0.8117647, 0.8117647, 0.8117647, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); // gray81
	for (int i = 0; i < 360; i += 60) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, -2.0, 1.62) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(0.5, 0.85, 0.8)); // 0.35
		glDrawArrays(GL_TRIANGLES, 36 * 1, POINTS_PER_CUBE);
	}
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // black
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, -2.0, -1.61) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(1.0, 1.7, 0.8));
		glDrawArrays(GL_TRIANGLES, 36 * 7, POINTS_PER_CUBE);
	}
	initReflection(0.8117647, 0.8117647, 0.8117647, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); // gray81
	for (int i = 0; i < 360; i += 60) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(0.0, -2.0, -1.62) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(0.5, 0.85, 0.8)); // 0.35
		glDrawArrays(GL_TRIANGLES, 36 * 1, POINTS_PER_CUBE);
	}
	// back wheel
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // black
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-10.0, -2.0, 1.61) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(1.0, 1.7, 0.8)); // 0.35
		glDrawArrays(GL_TRIANGLES, 36 * 7, POINTS_PER_CUBE);
	}
	initReflection(0.8117647, 0.8117647, 0.8117647, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); // gray81
	for (int i = 0; i < 360; i += 60) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-10.0, -2.0, 1.62) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(0.5, 0.85, 0.8)); // 0.35
		glDrawArrays(GL_TRIANGLES, 36 * 1, POINTS_PER_CUBE);
	}
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0); // black
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-10.0, -2.0, -1.61) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(1.0, 1.7, 0.8));
		glDrawArrays(GL_TRIANGLES, 36 * 7, POINTS_PER_CUBE);
	}
	initReflection(0.8117647, 0.8117647, 0.8117647, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0); // gray81
	for (int i = 0; i < 360; i += 60) {
		glUniformMatrix4fv(model, 1, GL_TRUE, carCtm * Translate(-10.0, -2.0, -1.62) * RotateZ(i * 1.0) * RotateZ(wheelRotZ * factor) * Scale(0.5, 0.85, 0.8)); // 0.35
		glDrawArrays(GL_TRIANGLES, 36 * 1, POINTS_PER_CUBE);
	}
}

void drawCar(mat4 carCtm) {
	drawCabin(carCtm);
	drawTrunk(carCtm, 0);
	drawWheel(carCtm, 0);
}

//Sword
void drawSword(mat4 swordCtm) {
	// handling component Orange2	(238 154 0)
	initReflection(0.93333334, 0.6039216, 0.0, 0.7, 0.7, 0.7, 1.0, 1.0, 1.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.024, -2.9, 0.0) * Scale(1.3, 1.0, 0.6)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.024, -3.9, 0.0) * Scale(1.3, 1.0, 0.6)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.024, -4.9, 0.0) * Scale(1.3, 1.0, 0.6)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// protector Bisque3	(205 183 158)
	initReflection(0.8039216, 0.7176471, 0.61960787, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(1.05, -2.8, 0.0) * RotateY(360) * RotateZ(75) * Scale(0.2, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(1.7, -3.1, 0.0) * RotateZ(45) * Scale(0.2, 0.6, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(1.97, -3.45, 0.0) * RotateZ(25) * Scale(0.19, 0.4, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(1.05, -5.0, 0.0) * RotateZ(-75) * Scale(0.2, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(1.7, -4.7, 0.0) * RotateZ(-45) * Scale(0.2, 0.6, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(1.97, -4.35, 0.0) * RotateZ(-25) * Scale(0.19, 0.4, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * RotateY(180) * Translate(2.05, -3.9, 0.0) * Scale(0.19, 0.6, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(1.0, -2.5, 0.0) * Scale(1.0, 0.2, 0.2)); 
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	// 
	// blade Gray71	(181 181 181)
	initReflection(0.70980394, 0.70980394, 0.70980394, 0.7, 0.7, 0.7, 1.0, 1.0, 1.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.15, -1.9, 0.0) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.15, -0.9, 0.0) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.13, 0.07, 0.0) * RotateZ(-3) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(-0.05, 1.0, 0.0) * RotateZ(-5) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.06, 1.95, 0.0) * RotateZ(-7) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.25, 2.9, 0.0) * RotateZ(-15) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.6, 3.85, 0.0) * RotateZ(-25) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(1.1, 4.6, 0.0) * RotateZ(-40) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(1.8, 5.3, 0.0) * RotateZ(-50) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(2.6, 5.8, 0.0) * RotateZ(-65) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(3.4, 6.05, 0.0) * RotateZ(-80) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(3.9, 6.12, 0.0) * Scale(0.5, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(4.0, 5.63, 0.0) * Scale(0.3, 1.4, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(3.4, 5.25, 0.0) * RotateZ(-70) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(2.7, 4.9, 0.0) * RotateZ(-60) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(2.0, 4.35, 0.0) * RotateZ(-40) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(1.55, 3.7, 0.0) * RotateZ(-30) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(1.15, 3.0, 0.0) * RotateZ(-25) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.75, 2.1, 0.0) * RotateZ(-20) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.5, 1.3, 0.0) * RotateZ(-15) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.35, 0.5, 0.0) * RotateZ(-10) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.2, -0.5, 0.0) * RotateZ(-7) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.1, -1.5, 0.0) * RotateZ(-5) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, swordCtm * Translate(0.12, -1.9, 0.0) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
}

// Rocket
GLfloat rocketRotY = 0.0;
void drawRocket(mat4 rocketCtm, int mode) {
	GLfloat factor = (mode == 1) ? 1.0 : 0.0;
	// lower SlateGray1	(198 226 255)
	initReflection(0.7764706, 0.8862745, 1.0, 0.0, 0.0, 0.0, 0.6, 0.3, 0.3);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * Translate(0.0, -1.5, 0.0) *  RotateY(i * 1.0 + rocketRotY * factor) * Scale(5.0, 3.0, 5.0));
		glDrawArrays(GL_TRIANGLES, 36 * 15, POINTS_PER_CUBE);
	}

	//// mid body
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * RotateY(i * 1.0 + rocketRotY * factor) * Scale(7.0, 9.0, 7.0));
		glDrawArrays(GL_TRIANGLES, 594, 18);
	}


	// upper IndianRed4	(139 58 58) 
	initReflection(0.54509807, 0.22745098, 0.22745098, 0.1, 0.1, 0.1, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * Translate(0.0, 5.45, 0.0) * RotateY(i * 1.0 + rocketRotY * factor) * Scale(3.9, 1.9, 3.9));
		glDrawArrays(GL_TRIANGLES, 576, 18);
	}

	// wings
	glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * RotateY(0.0 + rocketRotY * factor) * Translate(-3.5, -1.5, 0.0) * Scale(3.0, 3.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 576, 18);
	glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * RotateY(120 + rocketRotY * factor) * Translate(-3.5, -1.5, 0.0) * Scale(3.0, 3.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 576, 18);
	glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * RotateY(240 + rocketRotY * factor) * Translate(-3.5, -1.5, 0.0) * Scale(3.0, 3.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 576, 18);

	// window DeepSkyBlue4	(0 104 139)
	initReflection(0.0, 0.40784314, 0.54509807, 0.7, 0.7, 0.7, 1.0, 1.0, 1.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, rocketCtm * RotateZ(45 * factor) * RotateY(-30 + rocketRotY * factor) * Translate(0.0, 3.0, 2.65) * RotateX(90) * RotateY(i * 1.0) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 36, 36);
	}
}

//void drawBalloon() {
	/*for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, RotateY(i * 1.0) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, RotateY(i * 1.0) * RotateZ(90) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, RotateY(i * 1.0) * RotateZ(180) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, RotateY(i * 1.0) * RotateZ(270) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}*/
//}

// robot
void drawRobotHead(mat4 tranCtm) {
	// head DarkSlateBlue	(72 61 139)
	initReflection(0.28235295, 0.23921569, 0.54509807, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * RotateY(i * 1.0) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}

	// Scarves Tomato3	(205 79 57)
	initReflection(0.8039216, 0.30980393, 0.22352941, 0.1, 0.2, 0.2, 0.15, 0.2, 0.1);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * Translate(0.0, -0.1, 0.0) * RotateX(90) * Scale(1.0, 1.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * Translate(0.0, -0.1, 0.0) * RotateX(90) * RotateZ(90) * Scale(1.0, 1.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * Translate(0.0, -0.1, 0.0) * RotateX(90) * RotateZ(180) * Scale(1.0, 1.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * Translate(0.0, -0.1, 0.0) * RotateX(90) * RotateZ(270) * Scale(1.0, 1.0, 1.0));
	glDrawArrays(GL_TRIANGLES, 612, 108);

	// eye Coral1	(255 114 86)
	initReflection(1.0, 0.44705883, 0.3372549, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * Translate(0.0, 0.4, 0.4) * RotateX(90) * Scale(0.6, 0.6, 1.0));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranCtm * Translate(0.0, 0.4, 0.4) * RotateX(90) * RotateZ(90) * Scale(0.6, 0.6, 1.0));
	glDrawArrays(GL_TRIANGLES, 612, 108);
}

void drawRobotBody(mat4 tranBodyCtm) {
	// Dark Cyan	(0 128 128)
	initReflection(0.0, 0.5019608, 0.5019608, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranBodyCtm * Scale(4.0, 4.0, 4.0));
	glDrawArrays(GL_TRIANGLES, 720, 48);
	initReflection(1.0, 0.44705883, 0.3372549, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, tranBodyCtm * Translate(-1.0, 1.0, 1.9) * RotateZ(i * 1.0) * Scale(0.5, 0.5, 0.5));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void drawRobotArm(mat4 tranArmCtm) {
	initReflection(1.0, 0.44705883, 0.3372549, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(-0.15, -1.9, 0.0) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(-0.15, -0.9, 0.0) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(-0.13, 0.07, 0.0) * RotateZ(-3) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(-0.05, 1.0, 0.0) * RotateZ(-5) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(0.06, 1.95, 0.0) * RotateZ(-7) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(0.25, 2.9, 0.0) * RotateZ(-15) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(0.6, 3.85, 0.0) * RotateZ(-25) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(1.1, 4.6, 0.0) * RotateZ(-40) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(1.8, 5.3, 0.0) * RotateZ(-50) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(2.6, 5.8, 0.0) * RotateZ(-65) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranArmCtm * Translate(3.4, 6.05, 0.0) * RotateZ(-80) * Scale(1.0, 1.0, 0.3)); // 0.43
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);
}

void drawRobotFinger(mat4 tranFinger) {
	initReflection(0.28235295, 0.23921569, 0.54509807, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * Translate(-0.45, 0.25, 0.0) * Scale(0.3, 0.5, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * Translate(-0.4, -0.1, 0.0) * RotateZ(20) * Scale(0.3, 0.3, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * Translate(-0.27, -0.23, 0.0) * RotateZ(63) * Scale(0.3, 0.3, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * Translate(-0.1, -0.28, 0.0) * RotateZ(80) * Scale(0.3, 0.3, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * RotateY(180) * Translate(-0.45, 0.25, 0.0) * Scale(0.3, 0.5, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * RotateY(180) * Translate(-0.4, -0.1, 0.0) * RotateZ(20) * Scale(0.3, 0.3, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * RotateY(180) * Translate(-0.27, -0.23, 0.0) * RotateZ(63) * Scale(0.3, 0.3, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFinger * RotateY(180) * Translate(-0.1, -0.28, 0.0) * RotateZ(80) * Scale(0.3, 0.3, 1.0));
	glDrawArrays(GL_TRIANGLES, 72, 36);
}

void drawRobotFoot(mat4 tranFoot) {
	// leg
	initReflection(0.28235295, 0.23921569, 0.54509807, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, tranFoot * RotateY(i * 1.0) * Scale(1.0, 4.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 72, 36);
	}
	// foot
	initReflection(1.0, 0.44705883, 0.3372549, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, tranFoot * Translate(0.0, -1.5, 0.0) * Scale(2.0, 2.0, 2.0));
	glDrawArrays(GL_TRIANGLES, 576, 18);
}

void drawRobot(mat4 robotCtm) {
	// head
	drawRobotHead(robotCtm * Translate(0.0, 4.35, 1.8) * Scale(1.2, 1.2, 1.3));
	// body
	drawRobotBody(robotCtm * Translate(0.0, 2.0, 2.0));
	// arm
	drawRobotArm(robotCtm * Translate(-2.0, 3.0, 2.0) * RotateZ(85) * Scale(0.5, 0.5, 1.0));
	drawRobotArm(robotCtm * Translate(2.0, 3.0, 2.0) * RotateY(180) * RotateZ(85) * Scale(0.5, 0.5, 1.0));
	// fingers
	drawRobotFinger(robotCtm * Translate(-4.9, 5.5, 2.0));
	drawRobotFinger(robotCtm * Translate(4.9, 5.5, 2.0));
	// foots
	drawRobotFoot(robotCtm * Translate(-1.1, -2.0, 2.0) * Scale(0.6, 1.0, 0.6));
	drawRobotFoot(robotCtm * Translate(1.1, -2.0, 2.0) * Scale(0.6, 1.0, 0.6));
}


void drawDrum(mat4 drumCtm) {
	// surface LightYellow1	(255 255 224)
	initReflection(1.0, 1.0, 0.8784314, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(0.0, 0.4, 0.0) * RotateY(i * 1.0) * Scale(4.0, 0.8, 4.0));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// rope Tan	(210 180 140)
	initReflection(0.8235294, 0.7058824, 0.54901963, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(0.0, 0.3, 0.0) * RotateY(i * 1.0) * Scale(4.1, 0.2, 4.1));
		glDrawArrays(GL_TRIANGLES, 36, 36);
	}
	// body Turquoise2(0 229 238)
	initReflection(0.0, 0.8980392, 0.93333334, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(0.0, -0.9, 0.0) *  RotateY(i * 1.0) * Scale(4.0, 1.8, 4.0));
		glDrawArrays(GL_TRIANGLES, 72, 36);
	}
	// drumstick AntiqueGold	(221 196 136)
	initReflection(0.8666667, 0.76862746, 0.53333336, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(0.0, 0.95, 1.0) * Scale(3.0, 0.25, 0.25));
	glDrawArrays(GL_TRIANGLES, 36, 36);
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(1.375, 0.95, 1.0) * Scale(0.5, 0.5, 0.5)); // mau den
	glDrawArrays(GL_TRIANGLES, 72, 36);
	initReflection(0.8666667, 0.76862746, 0.53333336, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(0.0, 0.95, -1.0) * Scale(3.0, 0.25, 0.25));
	glDrawArrays(GL_TRIANGLES, 36, 36);
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, drumCtm * Translate(1.375, 0.95, -1.0) * Scale(0.5, 0.5, 0.5)); // mau den
	glDrawArrays(GL_TRIANGLES, 72, 36);
}

void drawToysShelf(mat4 tranCtm) {
	// Shelf - OK
	Shelf shelf(tranCtm); 
	shelf.drawObject();
	// Car - OK
	drawCar(tranCtm * Translate(2.0, 1.8, 2.0) * RotateY(180) * Scale(0.5, 0.5, 0.5));
	//Sword - OK
	drawSword(tranCtm * Translate(-4.0, 2.0, 2.0) * RotateZ(120) * Scale(0.6, 0.6, 0.6));
	// Rocket - OK
	drawRocket(tranCtm * Translate(-6.0, -3.25, 2.0) * Scale(0.4, 0.4, 0.4), 0);
	// Robot
	drawRobot(tranCtm * Translate(0.0, -2.7, 2.0) * Scale(0.4, 0.4, 0.4));
	// drum
	drawDrum(tranCtm * Translate(6.0, -3.0, 2.2) * Scale(0.6, 0.7, 0.6));
}

void drawComputer(mat4 tableCtm) {
	// computer arm
	initReflection(0.40784314, 0.5137255, 0.54509807, 0.4, 0.4, 0.4, 0.95, 0.95, 0.95); // lightblue4
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * RotateY(i * 1.0) * Scale(0.7, 3.0, 0.7));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	initReflection(0.40784314, 0.5137255, 0.54509807, 0.4, 0.4, 0.4, 0.95, 0.95, 0.95); // lightblue4
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(0.0, 1.8, -0.58) * RotateX(-60) * RotateY(i * 1.0) * Scale(0.7, 2.0, 0.7));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	initReflection(0.40784314, 0.5137255, 0.54509807, 0.4, 0.4, 0.4, 0.95, 0.95, 0.95); // lightblue4
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(0.0, 2.3, -1.7) * RotateX(90) * RotateY(i * 1.0) * Scale(0.7, 1.0, 0.7));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// screen
	initReflection(0.0, 0.0, 0.0, 0.2, 0.2, 0.2, 0.4, 0.4, 0.4); // black
	glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(0.0, 2.5, -2.3) * Scale(4.0, 3.0, 0.25));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawPaymentTable(mat4 tableCtm) {
	// left foot
	initReflection(0.54509807, 0.3529412, 0.16862746, 0.4, 0.4, 0.4, 0.7, 0.7, 0.7); // tan4
	glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(-9.9, 0.0, 0.0) * RotateY(0) * Scale(0.25, 10.0, 10.0)); 
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// surface
	initReflection(0.54509807, 0.3529412, 0.16862746, 0.4, 0.4, 0.4, 0.7, 0.7, 0.7); // tan4
	glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(0.0, 4.875, 0.0) * RotateY(0) * Scale(20, 0.25, 10.0)); 
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// right foot
	initReflection(0.54509807, 0.3529412, 0.16862746, 0.4, 0.4, 0.4, 0.7, 0.7, 0.7); // tan4
	glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(9.9, 0.0, 0.0) * RotateY(0) * Scale(0.25, 10.0, 10.0)); 
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// front foot
	initReflection(0.54509807, 0.3529412, 0.16862746, 0.4, 0.4, 0.4, 0.7, 0.7, 0.7); // tan4
	glUniformMatrix4fv(model, 1, GL_TRUE, tableCtm * Translate(0.0, 0.0, 4.876) * RotateY(0) * Scale(20.0, 10.0, 0.25)); 
	glDrawArrays(GL_TRIANGLES, 0, 36);
	drawComputer(tableCtm * Translate(0.0, 6.5, 0.0));
}

void drawAsisstantHead(mat4 assemblerCtm) {
	// Hat Rim RoyalBlue4	(39 64 139)
	initReflection(0.15294118, 0.2509804, 0.54509807, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.0, 0.5, 0.7) * RotateY(i * 1.0) * Scale(1.2, 0.1, 1.2));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// hat VioletRed2	(238 58 140) 
	initReflection(0.93333334, 0.22745098, 0.54901963, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.0, 0.4, 0.0) * RotateY(i * 1.0) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}
	// head Seashell1	(255 245 238)
	initReflection(1.0, 0.9607843, 0.93333334, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(i * 1.0) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(i * 1.0) * RotateZ(90) * Scale(1.0, 1.0, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(i * 1.0) * RotateZ(180) * Scale(1.0, 1.4, 1.0));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(i * 1.0) * RotateZ(270) * Scale(1.4, 1.0, 1.4));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}
	// left eye
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-0.4, 0.1, 1.0) * Scale(0.18, 0.08, 0.18));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-0.4, 0.1, 1.0) * RotateZ(90) * Scale(0.08, 0.18, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-0.4, 0.1, 1.0) * RotateZ(180) * Scale(0.18, 0.08, 0.18));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-0.4, 0.1, 1.0) * RotateZ(270) * Scale(0.08, 0.18, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	// right eye
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.4, 0.1, 1.0) * Scale(0.18, 0.08, 0.18));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.4, 0.1, 1.0) * RotateZ(90) * Scale(0.08, 0.18, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.4, 0.1, 1.0) * RotateZ(180) * Scale(0.18, 0.08, 0.18));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.4, 0.1, 1.0) * RotateZ(270) * Scale(0.08, 0.18, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	// nose
	initReflection(0.93333334, 0.79607844, 0.6784314, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.0, -0.25, 1.0) * Scale(0.1, 0.5, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// mouth IndianRed1	(255 106 106)
	initReflection(1.0, 0.41568628, 0.41568628, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.0, -0.6, 0.95) * RotateZ(180) * Scale(0.25, 0.2, 0.25));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(0.0, -0.6, 0.95) * RotateZ(270) * Scale(0.2, 0.25, 0.2));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	// ears PeachPuff2	(238 203 173)
	initReflection(0.93333334, 0.79607844, 0.6784314, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-1.0, 0.0, 0.0) * RotateZ(90) * Scale(0.5 * 0.45, 0.5 * 0.45, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-1.0, 0.0, 0.0) * RotateZ(180) * Scale(0.5 * 0.45, 1.1 * 0.5, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(180) * Translate(-1.0, 0.0, 0.0) * RotateZ(90) * Scale(0.5 * 0.45, 0.5 * 0.45, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(180) * Translate(-1.0, 0.0, 0.0) * RotateZ(180) * Scale(0.5 * 0.45, 1.1 * 0.5, 0.08));
	glDrawArrays(GL_TRIANGLES, 612, 108);
}

void drawAsisstantNeck(mat4 assemblerCtm) {
	initReflection(0.93333334, 0.79607844, 0.6784314, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * RotateY(i * 1.0) * Scale(0.6, 0.6, 0.6));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void drawAsisstantBody(mat4 assemblerCtm) {
	initReflection(0.4862745, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Scale(2.0, 4.5, 2.0));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	initReflection(0.4862745, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(-1.15, 0.0, 0.0) * RotateZ(9) * RotateY(i * 1.0) * Scale(1.45, 4.5, 1.45));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerCtm * Translate(1.15, 0.0, 0.0) * RotateZ(-9) * RotateY(i * 1.0) * Scale(1.45, 4.5, 1.45));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void drawAsisstantArm(mat4 assemblerArm) { 
	 initReflection(0.4862745, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) { // khop noi giua vai va bap tay
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * Translate(0.0, 2.5, 0.0) * RotateY(i * 1.0) * Scale(0.75, 0.75, 0.75));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * Translate(0.0, 2.5, 0.0) * RotateY(i * 1.0) * RotateZ(90) * Scale(0.75, 0.75, 0.75));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}

	initReflection(0.4862745, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < 360; i++) { // bap tay
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * Translate(0.0, 2.5, 0.0) * RotateX(180) * RotateY(i * 1.0) * Scale(1.5, 5.0, 1.5)); // - 2.5
		glDrawArrays(GL_TRIANGLES, 594, 18);
	}

	for (int i = 0; i < 360; i++) { 
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * RotateY(i * 1.0) * Scale(0.5, 0.5, 0.5)); // -2.5
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * RotateY(i * 1.0) * RotateZ(90) * Scale(0.55, 0.55, 0.55));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * RotateY(i * 1.0) * RotateZ(180) * Scale(0.55, 0.55, 0.55));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * RotateY(i * 1.0) * RotateZ(270) * Scale(0.55, 0.55, 0.55));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}

	for (int i = 0; i < 360; i++) { // cang tay gioi han 160 do Z va 180 do X
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerArm * RotateX(180) * RotateY(i * 1.0) * Scale(1.2, 7.0, 1.2));
		glDrawArrays(GL_TRIANGLES, 594, 18);
	}
}

void drawAsisstantHand(mat4 assemblerHand ,int side) { // In default , Object is right hand
	GLfloat factor = (side == 0) ? 1.0 : 0.0; // side = 0 => left hand => reverse right hand => lef hand
	// ban tay
	initReflection(0.93333334, 0.79607844, 0.6784314, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerHand * RotateX(180) * Translate(0.0, 4.0, 0.0) * Scale(1.3, 1.3, 0.4));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	initReflection(0.93333334, 0.79607844, 0.6784314, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) { // ngon ut
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerHand * RotateX(180) * RotateY(180 * factor) * Translate(-0.54, 5.05, 0.0) * RotateY(i * 1.0) * Scale(0.12, 0.8, 0.12));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	for (int i = 0; i < 360; i++) { // ngon ap ut
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerHand * RotateX(180) * RotateY(180 * factor) * Translate(-0.25, 5.25, 0.0) * RotateY(i * 1.0) * Scale(0.15, 1.2, 0.15));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	for (int i = 0; i < 360; i++) { // ngon giua
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerHand * RotateX(180) * RotateY(180 * factor) * Translate(0.1, 5.35, 0.0) * RotateY(i * 1.0) * Scale(0.18, 1.4, 0.18));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	for (int i = 0; i < 360; i++) { // ngon tro
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerHand * RotateX(180) * RotateY(180 * factor) * Translate(0.5, 5.25, 0.0) * RotateY(i * 1.0) * Scale(0.15, 1.2, 0.15));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	for (int i = 0; i < 360; i++) { // ngon cai
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerHand * RotateX(180) * RotateY(180 * factor) * Translate(0.73, 4.4, 0.0) * RotateZ(-30) * RotateY(i * 1.0) * Scale(0.23, 1.0, 0.23));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void drawAsisstantLeg(mat4 assemblerLeg) { // Khop noi chan o goc toa do ) => khi xoay khong can translate ve tam O
	initReflection(0.6117647, 0.6117647, 0.6117647, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) { // bap chan
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerLeg * Translate(0.0, -3.0, 0.0) * RotateX(180) * RotateY(i * 1.0) * Scale(2.0, 6.0, 2.0));
		glDrawArrays(GL_TRIANGLES, 594, 18);
	}
	// khop noi
	initReflection(0.6117647, 0.6117647, 0.6117647, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) {
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerLeg * Translate(0.0, -6.0, 0.0) * RotateY(i * 1.0) * Scale(0.75, 0.75, 0.75));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerLeg * Translate(0.0, -6.0, 0.0) * RotateY(i * 1.0) * RotateZ(90) * Scale(0.75, 0.75, 0.75));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerLeg * Translate(0.0, -6.0, 0.0) * RotateY(i * 1.0) * RotateZ(180) * Scale(0.75, 0.75, 0.75));
		glDrawArrays(GL_TRIANGLES, 612, 108);
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerLeg * Translate(0.0, -6.0, 0.0) * RotateY(i * 1.0) * RotateZ(270) * Scale(0.75, 0.75, 0.75));
		glDrawArrays(GL_TRIANGLES, 612, 108);
	}
	// cang chan
	initReflection(0.6117647, 0.6117647, 0.6117647, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) { // cang chan
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerLeg * Translate(0.0, -6.0, 0.0) * RotateX(180) * RotateY(i * 1.0) * Scale(1.6, 7.5, 1.6));
		glDrawArrays(GL_TRIANGLES, 594, 18);
	}
}

void drawShoes(mat4 assemblerShoes) {
	// de DarkGoldenrod3	(205 149 12)
	initReflection(0.8039216, 0.58431375, 0.047058824, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerShoes * Translate(0.0, -1.5, -0.5) * RotateX(-90) * Scale(3.0, 5.0, 0.5));
	glDrawArrays(GL_TRIANGLES, 768, 36);
	// than
	initReflection(0.0, 0.0, 0.0, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerShoes * Translate(0.0, 0.0, -0.5) * RotateX(-90) * Scale(3.0, 5.0, 2.5));
	glDrawArrays(GL_TRIANGLES, 804, 36);
	// cho xo chan
	initReflection(0.0, 0.0, 0.0, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	for (int i = 0; i < 360; i++) { 
		glUniformMatrix4fv(model, 1, GL_TRUE, assemblerShoes * Translate(0.0, -0.2, -4.5) * RotateY(i * 1.0) * Scale(2.15, 3.05, 2.15));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	initReflection(0.0, 0.0, 0.0, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerShoes * Translate(0.0, -0.225, -3.75) * Scale(3.0, 3.0, 1.5));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawAsisstant(mat4 asisstantTran) {
	drawAsisstantHead(asisstantTran * Translate(0.0, 2.5, 0.0));
	drawAsisstantNeck(asisstantTran * Translate(0.0, 1.0, 0.0));
	drawAsisstantBody(asisstantTran * Translate(0.0, -1.5, 0.0));
	// tay phai
	drawAsisstantArm(asisstantTran * Translate(-2.6, -1.6, 0.0) * Scale(0.7, 0.7, 0.7));
	drawAsisstantHand(asisstantTran * Translate(-2.6, -2.0, 0.0) * Scale(0.6, 0.6, 0.6), 1);
	// tay trai
	drawAsisstantArm(asisstantTran * Translate(2.6, -1.6, 0.0) * Scale(0.7, 0.7, 0.7));
	drawAsisstantHand(asisstantTran * Translate(2.6, -2.0, 0.0) * Scale(0.6, 0.6, 0.6), 0);
	// chan phai
	drawAsisstantLeg(asisstantTran * Translate(-0.85, -0.7, 0.0));
	drawShoes(asisstantTran * Translate(-0.85, -10.7, 1.4) * Scale(0.35, 0.25, 0.35));
	// chan trai
	drawAsisstantLeg(asisstantTran * Translate(0.85, -0.7, 0.0));
	drawShoes(asisstantTran * Translate(0.85, -10.7, 1.4) * Scale(0.35, 0.25, 0.35));
}

GLfloat doorRotY = 0.0;
void drawDoor(mat4 assemblerDoor) {

	// khung cua 
	initReflection(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerDoor * Translate(0.0, 13.0, 0) * Scale(20, 26, 0.99));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	// canh trai (theo thuc te) (quay am)
	initReflection(0.2, 0.4, 1.0, 0.05, 0.05, 0.05, 0.05, 0.05, 0.05);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerDoor * Translate(-10.0, 0.0, 0) * RotateY(doorRotY) * Translate(5.0, 13.0, 0) * Scale(10, 26, 1.0));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//canh phai (theo thuc te quay duong)
	initReflection(0.2, 0.4, 1.0, 0.3, 0.3, 0.3, 0.1, 0.3, 0.7);
	glUniformMatrix4fv(model, 1, GL_TRUE, assemblerDoor * Translate(10, 0.0, 0) * RotateY(-doorRotY) * Translate(-5.0, 13.0, 0) * Scale(10, 26, 1.0));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

// motion objects
GLfloat carXTran = -23.0, carYTran = -23.5, carZTran = -7.75;
void drawMotionCar() {
	drawCabin(Translate(carXTran, carYTran, carZTran) * Scale(0.5, 0.5, 0.5));
	drawTrunk(Translate(carXTran, carYTran, carZTran) * Scale(0.5, 0.5, 0.5), 1);
	drawWheel(Translate(carXTran, carYTran, carZTran) * Scale(0.5, 0.5, 0.5), 1);
}

/*Bac*/
mat4 model_xetai; // Chuyển động của cả xe (Cha)
void khungxe()
{
	initReflection(0.0, 0.54509807, 0.27058825, 0.4, 0.4, 0.4, 0.9, 0.9, 0.9);

	mat4 instance = Translate(0, 0, 0) * Scale(8, 0.2, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(0, -0.2, 0) * Scale(8, 0.5, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-3.95, 1, 1) * Scale(0.1, 2, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-3.95, 1, -1) * Scale(0.1, 2, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-1.98, 1, 0) * Scale(0.04, 2, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-3.98, 0.6, 0) * Scale(0.04, 1.2, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-3, 2, 0) * Scale(2, 0.04, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_xetai * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
mat4 model_cuaxe1, model_cuaxe2;
void cuaxe1()
{
	mat4 instance = Translate(0.95, 0, 0) * Scale(1.9, 1.2, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_cuaxe1 * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
void cuaxe2()
{
	mat4 instance = Translate(0.95, 0, 0) * Scale(1.9, 1.2, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_cuaxe2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
mat4 model_thungxe;
void khungthungxe()
{
	initReflection(0.93333334, 0.2509804, 0.0, 0.3, 0.0, 0.0, 1.0, 1.0, 1.0);

	mat4 instance = Translate(-2.8, 0, 0) * Scale(5.6, 0.1, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_thungxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-2.8, 1, 0.95) * Scale(5.6, 2, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_thungxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-2.8, 1, -0.95) * Scale(5.6, 2, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_thungxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

	instance = Translate(-5.6, 1, 0) * Scale(0.04, 2, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_thungxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
mat4 model_napthung;
void napthung()
{
	mat4 instance = Translate(0, -1, 0) * Scale(0.04, 2, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_napthung * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
GLfloat gocthung = 0.0, gocnapthung = 0.0; 
GLfloat goccua1 = 0.0, goccua2 = 0.0;

mat4 model_banhxe;
void banhxe()
{
	initReflection(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	mat4 instance = Translate(0, 0, -1) * Scale(0.8, 0.8, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_banhxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(0, 0, 1) * Scale(0.8, 0.8, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_banhxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);

}
GLfloat banhxequay = 0.0, xetaidc = 0.0;
void xetai()
{

	model_xetai = Translate(xetaidc, 0, 0) * Translate(0.5, -34.0, -0.8) * Scale(0.7, 0.7, 0.7);
	khungxe();
	model_cuaxe1 = model_xetai * Translate(-3.9, 0.7, 1) * RotateY(goccua1); //--
	cuaxe1();
	model_cuaxe2 = model_xetai * Translate(-3.9, 0.7, -1) * RotateY(goccua2); //++
	cuaxe2();

	model_thungxe = model_xetai * Translate(4, 0.1, 0) * RotateZ(gocthung);
	khungthungxe();
	model_napthung = model_thungxe * Translate(0, 2, 0) * RotateZ(gocnapthung);
	napthung();

	model_banhxe = model_xetai * Translate(0, -0.5, 0) * RotateZ(banhxequay);
	banhxe();
	model_banhxe = model_xetai * Translate(3, -0.5, 0) * RotateZ(banhxequay);
	banhxe();
	model_banhxe = model_xetai * Translate(-3, -0.5, 0) * RotateZ(banhxequay);
	banhxe();
}

// xe cau
GLfloat banhxecau = 0.0, xecaudc = 0.0, goccuac1 = 0.0, goccuac2 = 0.0;
mat4 model_sauxecau;
void sauxecau()
{
	initReflection(0.93333334, 0.2509804, 0.0, 0.3, 0.0, 0.0, 1.0, 1.0, 1.0);
	mat4 instance = Translate(0, 0, 0) * Scale(5, 0.5, 2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(2, 0.5, 0.5) * Scale(1, 0.5, 1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(0, 1, 0.02) * Scale(3, 1.5, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(0, 1, 1.02) * Scale(3, 1.5, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-2, 0.5, 0.02) * Scale(1, 0.5, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-2, 1.7, 0.02) * Scale(1, 0.1, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-2.48, 0.5, 0.5) * Scale(0.04, 0.5, 1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(0.5, 1, 0.5) * Scale(2, 1.5, 1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-1.5, 1.73, 0.5) * Scale(2, 0.04, 1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-2.48, 1.63, 0.5) * Scale(0.04, 0.2, 1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-2.46, 1, 0.02) * Scale(0.1, 1.5, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-2.46, 1, 0.98) * Scale(0.1, 1.5, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(2, 0.45, -0.15) * Scale(0.4, 0.4, 0.3);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(2, 0.45, -0.85) * Scale(0.4, 0.4, 0.3);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_sauxecau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
mat4 model_caucuaxe, model_caucuaxe2;
void caucuaxe()
{
	initReflection(0.7764706, 0.8862745, 1.0, 0.0, 0.0, 0.0, 0.6, 0.3, 0.3);
	mat4 instance = Translate(-2.5, 0, 0) * Scale(5, 0.4, 0.4);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_caucuaxe * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
void caucuaxe2()
{
	mat4 instance = Scale(4, 0.2, 0.2);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_caucuaxe2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-1.95, -0.4, 0) * Scale(0.1, 0.8, 0.1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_caucuaxe2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-1.7, -0.6, 0) * Scale(0.1, 0.2, 0.1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_caucuaxe2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
	instance = Translate(-1.8, -0.75, 0) * Scale(0.3, 0.1, 0.1);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_caucuaxe2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
mat4 model_cuasaucau;
void cuasaucau()
{
	mat4 instance = Translate(0.45, 0, 0) * Scale(0.9, 0.5, 0.04);
	glUniformMatrix4fv(model, 1, GL_TRUE, model_cuasaucau * instance);
	glDrawArrays(GL_TRIANGLES, 0, POINTS_PER_CUBE);
}
GLfloat goccuacau = 0.0, banhxequay1 = 0.0;
GLfloat goccau = 0.0, cau2 = -3.2, goccau1 = 0;
void xecau()
{

	model_xetai = Translate(xecaudc, 0, 0) * Translate(0.5, -34.0, 6.0) * Scale(0.7, 0.7, 0.7);
	khungxe();
	model_cuaxe1 = model_xetai * Translate(-3.9, 0.7, 1) * RotateY(goccuac1); //--
	cuaxe1();
	model_cuaxe2 = model_xetai * Translate(-3.9, 0.7, -1) * RotateY(goccuac2); //++
	cuaxe2();
	model_sauxecau = model_xetai * Translate(1.5, 0.35, 0) * RotateY(goccau);
	sauxecau();

	model_cuasaucau = model_sauxecau * Translate(-2.41, 0.5, 0.98) * RotateY(goccuacau);
	cuasaucau();
	model_caucuaxe = model_sauxecau * Translate(2, 0.5, -0.5) * RotateZ(goccau1);
	caucuaxe();
	model_caucuaxe2 = model_caucuaxe * Translate(cau2, 0, 0);
	caucuaxe2();

	model_banhxe = model_xetai * Translate(0, -0.5, 0) * RotateZ(banhxequay1);
	banhxe();
	model_banhxe = model_xetai * Translate(3, -0.5, 0) * RotateZ(banhxequay1);
	banhxe();
	model_banhxe = model_xetai * Translate(-3, -0.5, 0) * RotateZ(banhxequay1);
	banhxe();
}


// configuration set up
void setupViewAndProjection() {
	// camera set up
	//vec4 eye = vec4(camXLoca, camYLoca, camZLoca, 1.0), at = vec4(0.0, 0.0, 0.0, 1.0), up = vec4(0.0, 1.0, 0.0, 1.0);
	cameraCtm = LookAt(eye, at, up);
	camChangingCtm = Translate(camXTran, camYTran, camZTran) * RotateX(camXRot) * RotateY(camYRot) * RotateZ(camZRot);
	glUniformMatrix4fv(camera, 1, GL_TRUE, cameraCtm * camChangingCtm);

	// projector set up
	projectorCtm = Frustum(leftLim, rightLim, botLim, topLim, nearLim, farLim);
	glUniformMatrix4fv(projector, 1, GL_TRUE, projectorCtm);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupViewAndProjection();

	// all store - OK
	StoreLayout store;
	store.drawObject();

	// Toys Shelfs
	/*drawToysShelf(Translate(-20.0, -30.0, 35));*/
	drawToysShelf(Translate(-20.0, -30.0, 20));
	drawToysShelf(Translate(-20.0, -30.0, 5));
	drawToysShelf(Translate(-20.0, -30.0, -10));
	drawToysShelf(Translate(-20.0, -30.0, -25));
	drawToysShelf(Translate(-20.0, -30.0, -40));
	drawToysShelf(Translate(20.0, -30.0, -10));
	drawToysShelf(Translate(20.0, -30.0, -25));
	drawToysShelf(Translate(20.0, -30.0, -40));

	// payment - OK
	drawPaymentTable(Translate(20.0, -31.0, 20.0) * RotateY(-90) * Scale(0.8, 0.8, 0.8));

	// motion car
	drawMotionCar();
	// motion Rocket
	drawRocket(Translate(21, -22.7, 26.0) * Scale(0.4, 0.4,0.4), 1);
	initReflection(0.0, 0.0, 0.0, 0.2, 0.2, 0.2, 0.4, 0.4, 0.4);
	glUniformMatrix4fv(model, 1, GL_TRUE, Translate(21, -24, 26.0) * Scale(0.3, 6.0, 0.3));
	glDrawArrays(GL_TRIANGLES, 36 * 0, POINTS_PER_CUBE);

	// asisstant
	drawAsisstant(Translate(27, -22.5, 20) * Scale(1.1, 1.1, 1.1) * RotateY(-90));
	drawAsisstant(Translate(24, -22.5, 5) * Scale(1.1, 1.1, 1.1) * RotateY(-30));

	// door - Bang
	drawDoor(Translate(0.0, -36.0, 46.9));

	// Bac
	xetai();
	xecau();

	glutSwapBuffers();
}

//float centerDistance = 0.0;
//float xCenterDistance = 0.0;
//float beta = 0.0;
//float delta = 0.0;
//float zDistance = 0.0;
//float xDistance = 0.0;
//GLfloat side = 0.0;
//
//void look(int direction) {
//	if (direction != 1 && direction != -1) return;
//	centerDistance = sqrt((eye.x - at.x) * (eye.x - at.x) + (eye.z - at.z) * (eye.z - at.z));
//	xCenterDistance = abs(at.x - eye.x);
//	beta = (side == 0.0) ? acos(xCenterDistance / centerDistance) * 180 / 3.14159265 : side;
//	delta = beta + 5 * direction;
//	zDistance = centerDistance * sin(beta * 3.14159265 / 180);
//	xDistance = centerDistance * cos(delta * 3.14159265 / 180);
//	at.x = eye.x - xDistance;
//	at.y = at.y;
//	at.z = eye.z - zDistance;
//	side = delta;
//}
//
//GLfloat increase = 1.0;
//GLfloat increaseXDis = 0.0, increaseZDis = 0.0;
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		/*Move camera : Translate camare go left , right , up, down*/
		case 'a': 
			camXTran = (eye.z >= at.z) ? camXTran + 1.0 : camXTran - 1.0;
			break;
		case 'w': 
			camZTran = (eye.z >= at.z) ? camZTran + 1.0 : camZTran - 1.0;
			break;
		case 'S':
			camYTran += 1.0;
			break;
		case 'd': 
			camXTran = (eye.z >= at.z) ? camXTran - 1.0 : camXTran + 1.0;
			break;
		case 's': 
			camZTran = (eye.z >= at.z) ? camZTran - 1.0 : camZTran + 1.0;
			break;
		case 'W':
			camYTran -= 1.0;
			break;

		/*It looks like you walk around one point*/
		case 'x': 
			camXRot += 10;
			camXRot = (camXRot > 360) ? camXRot - 360 : camXRot;
			break;
		case 'X':
			camXRot -= 10;
			camXRot = (camXRot < -360) ? camXRot + 360 : camXRot;
			break;
		case 'y': 
			camYRot += 10;
			camYRot = (camYRot > 360) ? camYRot - 360 : camYRot;
			break;
		case 'Y':
			camYRot -= 10;
			camYRot = (camYRot < -360) ? camYRot + 360 : camYRot;
			break;
		case 'z': 
			camZRot += 10;
			camZRot = (camZRot > 360) ? camZRot - 360 : camZRot;
			break;
		case 'Z': 
			camZRot -= 10;
			camZRot = (camZRot < -360) ? camZRot + 360 : camZRot;
			break;
		case 'o':
			if (doorRotY < 170) {
				doorRotY += 5;
			}
			break;
		case 'O':
			if (doorRotY > 0) {
				doorRotY -= 5;
			}
			break;
		/*Rotate Camera Direction : around Oy*/
		/*case 'l':
			look(-1);
			break;
		case 'r':
			look(1);
			break;*/
		case '<':
			rocketRotY -= 10;
			wheelRotZ -= 10;
			carXTran -= 0.5;
			break;
		case '>':
			rocketRotY += 10;
			wheelRotZ += 10;
			carXTran += 0.5;
			break;
		/*case '6':
			liftTrunkRotateZ += 10;
			topTrunkSheetYTran = 9.5 * sin(liftTrunkRotateZ * 3.14159265 / 180);
			topTrunkSheetXTran = 9.5 - (9.5 * cos(liftTrunkRotateZ * 3.14159265 / 180));
			break;*/
		case 'u':
			goccua1 -= 5;
			if (goccua1 < -80) goccua1 = -80;
			break;
		case 'U':
			goccua1 += 5;
			if (goccua1 > 0) goccua1 = 0;
			break;
		case 'i':
			goccua2 += 5;
			if (goccua2 > 80) goccua2 = 80;
			break;
		case 'I':
			goccua2 -= 5;
			if (goccua2 < 0) goccua2 = 0;
			break;
		case '1':
			gocthung -= 5;
			if (gocthung < -30) gocthung = -30;
			gocnapthung += 5;
			if (gocnapthung > 30) gocnapthung = 30;
			break;
		case '2':
			gocthung += 5;
			if (gocthung > 0) gocthung = 0;
			gocnapthung -= 5;
			if (gocnapthung < 0) gocnapthung = 0;
			break;
		case 'p':
			gocnapthung += 5;
			if (gocnapthung > 30) gocnapthung = 30;
			break;
		case 'P':
			gocnapthung -= 5;
			if (gocnapthung < 0) gocnapthung = 0;
			break;
		case '[':
			xetaidc -= 0.1;
			banhxequay += 10;
			break;
		case ']':
			xetaidc += 0.1;
			banhxequay -= 10;
			break;
		case '0':
			goccuacau -= 10;
			if (goccuacau < -90) goccuacau = -90;
			break;
		case ')':
			goccuacau += 10;
			if (goccuacau > 0) goccuacau = 0;
			break;
		case '9':
			goccau -= 10;
			break;
		case '(':
			goccau += 10;
			break;
		case '8':
			goccau1 -= 5;
			if (goccau1 < -60) goccau1 = -60;
			break;
		case '*':
			goccau1 += 5;
			if (goccau1 > 0) goccau1 = 0;
			break;
		case '7':
			cau2 -= 0.3;
			if (cau2 < -7) cau2 = -7;
			break;
		case '&':
			cau2 += 0.3;
			if (cau2 > -3.2) cau2 = -3.2;
			break;
		case '6':
			goccuac2 += 5;
			if (goccuac2 > 80) goccuac2 = 80;
			break;
		case '^':
			goccuac2 -= 5;
			if (goccuac2 < 0) goccuac2 = 0;
			break;
		case '5':
			goccuac1 -= 5;
			if (goccuac1 < -80) goccuac1 = -80;
			break;
		case '%':
			goccuac1 += 5;
			if (goccuac1 > 0) goccuac1 = 0;
			break;
		case '-':
			xecaudc -= 0.1;
			banhxequay1 += 10;
			break;
		case '=':
			xecaudc += 0.1;
			banhxequay1 -= 10;
			break;
	}
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	vec4 eye(camXLoca, camYLoca, camZLoca, 1.0);
	vec4 at(0.0, 0.0, 0.0, 1.0);
	vec4 up(0.0, 1.0, 0.0, 1.0);

	cameraCtm = LookAt(eye, at, up);
	glUniformMatrix4fv(camera, 1, GL_TRUE, cameraCtm);

	projectorCtm = Frustum(leftLim, rightLim, botLim, topLim, nearLim, farLim);
	glUniformMatrix4fv(projector, 1, GL_TRUE, projectorCtm);

	glViewport(0, 0, width, height);
}

int isUpward = 1;
int isLift = 1;
void automateMotion(void) {
	// rocket
	rocketRotY += 20;
	rocketRotY = (rocketRotY > 360) ? rocketRotY - 360 : rocketRotY;
	// car
	if (isUpward == 1 && isLift == 1) {
		if (carXTran < -13) {
			wheelRotZ += 10;
			carXTran += 0.5;
			isUpward = (carXTran < -13) ? 1 : 0;
		}
		if (carXTran >= -13) {
			isUpward = 1;
			liftTrunkRotateZ += 10;
			if (liftTrunkRotateZ == 70) {
				isUpward = 0;
			}
			topTrunkSheetYTran = 9.5 * sin(liftTrunkRotateZ * 3.14159265 / 180);
			topTrunkSheetXTran = 9.5 - (9.5 * cos(liftTrunkRotateZ * 3.14159265 / 180));
		}
	}
	else {
		if (isLift == 1 && liftTrunkRotateZ > 0) {
			liftTrunkRotateZ -= 10;
			topTrunkSheetYTran = 9.5 * sin(liftTrunkRotateZ * 3.14159265 / 180);
			topTrunkSheetXTran = 9.5 - (9.5 * cos(liftTrunkRotateZ * 3.14159265 / 180));
		}
		if (liftTrunkRotateZ == 0) { 
			isLift = 0;
			wheelRotZ -= 10;
			carXTran -= 0.5;
			if (carXTran == -23) {
				isUpward = 1;
				isLift = 1;
			}
		}
	}
	glutPostRedisplay();
}

int main( int argc, char **argv )
{
    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 800, 800 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Cua hang do choi" );            
   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );    
    glutKeyboardFunc( keyboard );  
	glutReshapeFunc(reshape);
	glutIdleFunc(automateMotion);

	glutMainLoop();
    return 0;
}