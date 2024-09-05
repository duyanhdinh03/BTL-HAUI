//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );

typedef vec4 point4;
typedef vec4 color4;
using namespace std;

// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/

point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/
vec3 normals[NumPoints];
GLuint program;
mat4 model;

GLuint model_loc,view_loc,projection_loc;

void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry( void )
{
	initCube();
	makeColorCube();
}


void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);
	
}


//void Color(float r, float g, float b, float r1, float g1, float b1, float cl) {
//	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
//	point4 light_position(0.0, 0.0, 1.0, 0.0);
//	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
//	color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
//	color4 light_specular(0.2, 0.2, 0.2, 1.0);
//
//	/*color4 material_ambient(1.0, 0.0, 1.0, 1.0);
//	color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
//	color4 material_specular(1.0, 0.8, 0.0, 1.0);*/
//
//	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
//	color4 material_diffuse(r, g, b);
//	color4 material_specular(r1, g1, b1);
//	float material_shininess = cl;
//
//	color4 ambient_product = light_ambient * material_ambient;
//	color4 diffuse_product = light_diffuse * material_diffuse;
//	color4 specular_product = light_specular * material_specular;
//
//	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
//	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
//	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
//	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
//	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
//}


GLfloat opacity = 1.0;
float Opacity = 1.0;
GLfloat mauAnhSang = 1.0;
GLfloat choiSang = 0.6;

void mau(GLfloat a, GLfloat b, GLfloat c) {
	/* Khởi tạo các tham số chiếu sáng - tô bóng*/
	point4 light_position(0.0, 2.0, 0.0, 1.0);
	color4 light_ambient(0.2, 0.2, 0.2, 1.0);
	color4 light_diffuse(mauAnhSang, mauAnhSang, mauAnhSang, 1.0);
	color4 light_specular(choiSang, choiSang, choiSang, 1.0);

	color4 material_ambient(1.0, 0.0, 1.0, 1.0);
	color4 material_diffuse(a / 255.0, b / 255.0, c / 255.0, 1.0);
	color4 material_specular(255.0 / 255.0, 255.0 / 255.0, 255 / 255.0, 1.0);
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
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
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

	model_loc = glGetUniformLocation(program, "Model");
	view_loc = glGetUniformLocation(program, "View");
	projection_loc = glGetUniformLocation(program, "Projection");
	

	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}

void drawCylinder(float radius, float height, int numSegments) {
	float segmentAngle = 2.0f * M_PI / numSegments;
	// Vẽ thành hình trụ
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= numSegments; i++) {
		float angle = i * segmentAngle;
		// Tính toán các tọa độ x, y của các điểm trên đường viền hình trụ
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		// Vẽ đỉnh dưới của hình trụ
		glVertex3f(x, y, 0.0f);
		// Vẽ đỉnh trên của hình trụ
		glVertex3f(x, y, height);
	}
	glEnd();

	// Vẽ đáy dưới của hình trụ
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, 0.0f);
	for (int i = 0; i <= numSegments; i++) {
		float angle = i * segmentAngle;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

	// Vẽ đáy trên của hình trụ
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0f, 0.0f, height);
	for (int i = 0; i <= numSegments; i++) {
		float angle = i * segmentAngle;
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		glVertex3f(x, y, height);
	}
	glEnd();
}

void hinhtru() {
	mat4 instance = Scale(0.5, 0.5, 0.5) * Translate(-1, -1, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	drawCylinder(1.0f, 1.0f, 100); // Vẽ hình trụ tròn
}

////////////////////////////////////////// TẠO PHÒNG BẾP ///////////////////////////////////////////////////////////////////////////////
///// Khung phòng //////
mat4 model_cuaChinh, model_cuaSo, model_quat;
GLfloat goccua, gocso = 65, quayQuat;
void sanNha() {
	mat4 sannha = Scale(6, 0.01, 8);

	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * sannha);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	
}
void tranNha() {
	mat4 sannha = Translate(0, 4, 0) * Scale(6, 0.01, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * sannha);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongTrai() {
	mat4 tuongtrai = Translate(-3, 2, 0) * Scale(0.01, 4, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongtrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongPhai() {
	mat4 tuongphai = Translate(3, 2, 0) * Scale(0.01, 4, 8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongphai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongTruoc1() {
	
	mat4 tuongtruoc = Translate(-2, 2, 4) * Scale(2, 4, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongtruoc);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongTruoc2() {
	
	mat4 tuongtruoc = Translate(2, 2, 4) * Scale(2, 4, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongtruoc);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongTruoc3() {

	mat4 tuongtruoc = Translate(0, 3, 4) * Scale(2, 2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongtruoc);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongSau1() {
	mat4 tuongsau = Translate(-1.75, 2, -4) * Scale(2.5, 4, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongsau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongSau2() {
	mat4 tuongsau = Translate(2, 2, -4) * Scale(2, 4, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongsau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongSau3() {
	mat4 tuongsau = Translate(0.25, 0.5, -4) * Scale(1.5, 1, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongsau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuongSau4() {
	mat4 tuongsau = Translate(0.25, 3.25, -4) * Scale(1.5, 1.5, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * tuongsau);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khungPhong() {
	//mau(225, 239, 148);
	mau(225, 296, 155);
	sanNha();
	
	tuongTrai();
	tuongPhai();
	tuongTruoc1();
	tuongTruoc2();
	tuongTruoc3();
	tuongSau1();
	tuongSau2();
	tuongSau3();
	tuongSau4();
	mau(250, 250, 250);
	tranNha();
}
///// Cửa ra vào và cửa số //////

void cuaTrai()
{
	mat4 cuatrai =  Translate(0.5, 0, 0) * Scale(1, 2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_cuaChinh * cuatrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cuaPhai()
{
	mat4 cuaphai = Translate(-0.5, 0, 0)* Scale(1, 2, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,model_cuaChinh * cuaphai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cuaSoTrai()
{
	mat4 cuasotrai = Translate(-0.75, 0, 0) * RotateY(gocso) * Translate(0.375, 0, 0) * Scale(0.75, 1.5, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_cuaSo * cuasotrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void cuaSoPhai()
{
	mat4 cuasophai = Translate(0.75, 0, 0) * RotateY(-gocso) * Translate(-0.375, 0, 0) * Scale(0.75, 1.5, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_cuaSo * cuasophai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tayNamCua1(GLfloat traiPhai)
{
	mat4 cuatrai = Translate(0.95 * traiPhai, 0.2, 0.05) * Scale(0.05, 0.05, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_cuaChinh * cuatrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tayNamCua2(GLfloat traiPhai)
{
	mat4 cuatrai = Translate(0.95 * traiPhai, -0.1, 0.05) * Scale(0.05, 0.05, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_cuaChinh * cuatrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tayNamCua3(GLfloat traiPhai)
{
	mat4 cuatrai = Translate(0.95 * traiPhai, 0.05, 0.1) * Scale(0.05, 0.35, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_cuaChinh * cuatrai);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void cuaPhong()
{
	
	mau(139, 90,0);
	model_cuaChinh = model *Translate(-1, 1, 4) * RotateY(goccua);
	cuaTrai();
	mau(176, 224, 230);
	tayNamCua1(1);
	tayNamCua2(1);
	tayNamCua3(1);
	mau(139, 90, 0);
	model_cuaChinh = model * Translate(1, 1, 4) * RotateY(-goccua);
	cuaPhai();
	mau(176, 224, 230);
	tayNamCua1(-1);
	tayNamCua2(-1);
	tayNamCua3(-1);
	mau(139, 90, 0);
	model_cuaSo = model * Translate(0.25, 1.75, -4);
	cuaSoTrai();
	cuaSoPhai();
}
///// Đèn điện //////
void thanhDen()
{
	mat4 thanhden = Translate(-2.95, 3.3, 0.5) * Scale(0.1, 0.2, 2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * thanhden);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhDenTrai()
{
	mat4 thanhden = Translate(-2.875, 3.3, 1.5) * Scale(0.2, 0.2, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * thanhden);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanhDenPhai()
{
	mat4 thanhden = Translate(-2.875, 3.3, -0.5) * Scale(0.2, 0.2, 0.05);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * thanhden);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bongDen() {
	mat4 instance = Translate(-2.8, 3.3, 0.5) * RotateZ(45) * Scale(0.1, 0.1, 2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
bool batDen = true;
void denDien()
{
	//mau(255, 255, 255);
	mau(176, 224, 230);
	thanhDen();
	thanhDenTrai();
	thanhDenPhai();
	if (batDen)
		mau(220, 230, 240);
	else
		mau(110, 120, 130);
	bongDen();
}
/////// Quạt trần ///////
void canh(float x)
{
	mat4 instance = RotateY(x) * Translate(0, 0, 0.65) * Scale(0.2, 0.02, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_quat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bocanh()
{
	canh(0);
	canh(120);
	canh(240);
}
void canhNho(float x)
{
	mat4 instance = RotateY(x) * Translate(0, 0, 0.2) * Scale(0.08, 0.02, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_quat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bocanhNho()
{
	canhNho(0);
	canhNho(120);
	canhNho(240);
}

GLboolean checked = false;
void canh_quay()
{
	if (checked) {
		quayQuat += 25;
		glutPostRedisplay();
	}
}
void trucQuay() {
	mat4 instance = Translate(0,0, 0) * RotateX(90);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_quat * instance);
	drawCylinder(0.15, 0.04, 100); // Vẽ hình trụ tròn
}
void trucOng() {
	mat4 instance = Translate(0, 0.4, 0) * RotateX(90);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_quat * instance);
	drawCylinder(0.02, 0.4, 100); // Vẽ hình trụ tròn
}
void trucTren()
{
	mat4 instance = Translate(0, 0.4, 0) * RotateY(45) * RotateX(35) * RotateZ(45) * Scale(0.1, 0.1, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_quat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void quatTran() {
	//mau(250, 250, 250);
	mau(176, 224, 230);
	model_quat = model * Translate(1, 3.5, 1.5);
	trucQuay();
	trucOng();
	trucTren();
	model_quat *= RotateY(quayQuat);
	bocanhNho();
	bocanh();
	canh_quay();
}

///// Trang trí //////
void thanhTivi()
{
	mat4 thanh = Translate(2.9, 1.5, 1.5) * Scale(0.2, 0.2, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tivi() {
	mat4 thanh = Translate(2.8, 1.5, 1.5) * Scale(0.1, 1, 1.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
GLfloat xee;
mat4 xe;
bool batTV = false;
void banhXe(GLfloat y, GLfloat z)
{
	mat4 instance = Translate(2.72, y, z) * RotateY(90);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, xe * instance);
	drawCylinder(0.1, 0.02, 100); // Vẽ hình trụ tròn
}
void mattroi()
{
	mat4 instance = Translate(2.72, 1.86, 2.1) * RotateY(90);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * instance);
	drawCylinder(0.1, 0.02, 100); // Vẽ hình trụ tròn
}
void tiaMatTroi(GLfloat y, GLfloat z, GLfloat a)
{
	mat4 thanh = Translate(2.72, y, z) * RotateX(a) * Scale(0.015, 0.015, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thanXe()
{
	mat4 thanh = Translate(2.72, 1.45, 1.3) * Scale(0.02, 0.15, 0.7);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, xe * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thanXe2()
{
	mat4 thanh = Translate(2.72, 1.55, 1.3) * Scale(0.02, 0.12, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, xe * thanh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void trangTri()
{
	if (batTV)mau(0, 0, 255);
	else
	{
		mau(0, 0, 0);
		xee = 0;
	}
	thanhTivi();
	tivi();
	mau(0, 0, 0);
	xe = model * Translate(0, 0, xee);
	banhXe(1.3, 1.1);
	banhXe(1.3, 1.5);
	thanXe();
	thanXe2();
	if (batTV && xee < 0.6)
	{
		xee += 0.0005;
		glutPostRedisplay();
	}
	mattroi();
	tiaMatTroi(1.86, 1.94, 0);
	tiaMatTroi(1.92, 1.98, 40);
	tiaMatTroi(1.8, 1.98, -40);
	tiaMatTroi(1.75, 2.05, -60);
	tiaMatTroi(1.72, 2.12, -90);
	tiaMatTroi(1.75, 2.18, -120);
}




void taoPhong()
{
	khungPhong();
	cuaPhong();
	denDien();
	quatTran();
	trangTri();
}


/////////////////////////////////////////////////// KHU BẾP ///////////////////////////////////////////////////////////////////////
mat4 canhtu_m;
mat4 bep_m;

mat4 tubep_m;
float tubepxyz[4] = { 0,0,0,0 };


void canhturuabat(GLfloat x, GLfloat y, GLfloat z, GLfloat lr, GLfloat sx, GLfloat sy, GLfloat sz) {
	mat4 temp = Scale(sx,sy,sz) * Translate(0, 0, lr);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * canhtu_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void matbep() {
	mat4 temp = Scale(1.4, 0.04, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khungtubep() {
	// thanh phai
	mat4 temp = Translate(0.64, -0.42, 0) * Scale(0.04, 0.8, 0.52);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//thanh trai
	temp = Translate(-0.64, -0.42, 0) * Scale(0.04, 0.8, 0.52);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// thanh sau
	temp = Translate(0, -0.42, 0.24) * Scale(1.24, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);

	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void beptu() {
	mat4 temp = Translate(0, 0.01, 0) * Scale(1, 0.04, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void taytu() {
	mat4 temp = Translate(0, 0.098, -0.32) * Scale(0.3, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(-0.13, 0.098, -0.28) * Scale(0.04, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(0.13, 0.098, -0.28) * Scale(0.04, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}




void xoong(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y + 0.25 - 0.04, z) * RotateX(90) * Scale(0.5, 0.5, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	drawCylinder(0.3f, 0.4f, 100); // Vẽ hình trụ tròn
}

//void cacloaihop(GLfloat x, GLfloat y, GLfloat z) {
//	mat4 instance = Translate(x, y , z) * RotateX(90) * Scale(0.5, 0.5, 0.5);
//	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
//	drawCylinder(0.2f, 0.3f, 100); // Vẽ hình trụ tròn
//}

void tubep() {
	// mat duoi
	mat4 temp = Scale(1.24, 0.04, 0.52);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// mat trai
	temp = Translate(-0.6, 0.196 / 2, 0) * Scale(0.04, 0.196, 0.52);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// mat phai
	temp = Translate(0.6, +0.196 / 2, 0) * Scale(0.04, 0.196, 0.52);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// mat truoc
	temp = Translate(0, +0.196 / 2, 0.24) * Scale(1.24, 0.196, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// mat sau
	temp = Translate(0, +0.196 / 2, -0.24) * Scale(1.24, 0.196, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * tubep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	taytu();
}

void botubep() {
	tubep_m = Translate(0, -0.2, 0) * Translate(0, 0, tubepxyz[0]);
	tubep();

	tubep_m = Translate(0, -0.4, 0) * Translate(0, 0, tubepxyz[1]);
	tubep();

	tubep_m = Translate(0, -0.6, 0) * Translate(0, 0, tubepxyz[2]);
	tubep();
	tubep_m = Translate(0, -0.8, 0) * Translate(0, 0, tubepxyz[3]);
	tubep();
}

void mattudungdo() {
	mat4 temp = Translate(1, 0, -0.8 - 2.3 + 0.6) * Scale(0.6, 0.04, 1.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}



void khungtudungdo() {
	

	mat4 temp = Translate(1 + 0.24, -0.4, -0.8 - 2.3 + 0.6 + 0.04) * Scale(0.04, 0.8, 1.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(1 + 0.24 - 0.52, -0.4, -0.8 - 2.3 + 0.04) * Scale(0.04, 0.8, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(1 + 0.24 - 0.52, -0.4, -0.8 - 2.3 + 0.04 + 0.4 + 0.72 + 0.08) * Scale(0.04, 0.8, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(0.24 + 0.7 + 0.04, -0.4, 0 - 3.7 + 0.6 - 0.1 - 0.04) * Scale(0.48, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

int alpha = 0;

void hecanhtu() {
	canhtu_m = Translate(1 + 0.24 - 0.52, -0.4, -0.8 - 2.3 + 0.04 + 0.4 + 0.72 + 0.08 - 0.2) * RotateY(alpha);
	canhturuabat(0, 0, 0, -0.5, 0.04, 0.8, 0.4);

	canhtu_m = Translate(1 + 0.24 - 0.52, -0.4, -0.8 - 2.3 + 0.04 + 0.4 + 0.72 + 0.08 - 0.2 - 0.72 - 0.08) * RotateY(-alpha);
	canhturuabat(0, 0, 0, 0.5, 0.04, 0.8, 0.4);
}



mat4 bonruabat_m;
mat4 canhtubonruabat_m;
mat4 voirua_m;
int voirua_q = 0;

void voirua_thanh1() {
	mat4 temp = Scale(0.04, 0.2, 0.04) * Translate(-0.5, 0.5, 0);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * voirua_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void voirua_thanh2() {
	mat4 temp = Translate(-0.05, 0.22, 0) * Scale(0.1, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * voirua_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void voirua(GLfloat x, GLfloat y, GLfloat z) {
	voirua_m = Translate(1.25, 0, -1) ;
	voirua_thanh1();
	voirua_m = voirua_m * RotateY(voirua_q);
	voirua_thanh2();

}

void bonruabat(GLfloat x, GLfloat y, GLfloat z) {
	// mat duoi
	mat4 temp = Translate(1.2 + x, 0 + y, 0 + z) * Scale(0.4, 0.04, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	//mat trai
	temp = Translate(1.2 + x, 0.2 + y, -0.38 + z) * Scale(0.4, 0.36, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	// mat phai
	temp = Translate(1.2 + x, 0.2 + y, 0.38 + z) * Scale(0.4, 0.36, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(1.2 + 0.18 + x, 0.2 + y, 0 + z) * Scale(0.04, 0.36, 0.72);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(1.2 - 0.18 + x, 0.2 + y, 0 + z) * Scale(0.04, 0.36, 0.72);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void matkhuruabat(GLfloat x, GLfloat y, GLfloat z) {
	mat4 temp = Translate(1 + x, 0 + y, -0.1 + 0.5 + z) * Scale(0.6, 0.04, 1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	

	temp = Translate(1 - 0.25 + x, 0 + y, -0.1 + 0.5 - 0.6 + z) * Scale(0.1, 0.04, 1.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(1 + 0.25 + x, 0 + y, -0.1 + 0.5 - 0.6 + z) * Scale(0.1, 0.04, 1.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(1 + x, 0 + y, -0.1 - 0.8 + z) * Scale(0.6, 0.04, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void mattubat(GLfloat x, GLfloat y, GLfloat z) {
	mat4 temp = Translate(1 + x, 0 + y, -0.1 + 0.5 + z) * Scale(0.6, 0.04, 3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}






void hecanhturuabat() {
	canhtu_m = Translate(0.68 + 0.56 - 0.52, -0.4, 0.18 - 0.12 - 0.8 + 0.18 + 0.02) * RotateY(alpha);
	canhturuabat(0, 0, 0, -0.5, 0.04, 0.8, 0.36);

	/* canh trai*/
	canhtu_m = Translate(0.68 + 0.56 - 0.52, -0.4, -0.54 - 0.12 - 0.8 + 0.18 + 0.02) * RotateY(-alpha);
	canhturuabat(0, 0, -0.18, 0.5, 0.04, 0.8, 0.36);
}


void khuturuabat() {
	mat4 temp = Translate(0.68 + 0.56, -0.4, -1.92 / 2 + 0.3 - 0.04) * Scale(0.04, 0.8, 1.92);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(0.68 + 0.56 - 0.52, -0.4, -0.8 / 2 + 0.3 - 0.04) * Scale(0.04, 0.8, 0.8);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(0.68 + 0.56 - 0.52, -0.4, -1.52 - 0.4 / 2 + 0.3 - 0.04) * Scale(0.04, 0.8, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(0.96, -0.4, 0.24) * Scale(0.6, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	temp = Translate(0.98, -0.4, -1.28 - 0.02 - 0.3 - 0.04) * Scale(0.48, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * temp);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khunaunuong() {
	mau(100, 120, 60);
	matbep();
	mau(70, 120, 60);
	khungtubep();

	mau(100, 120, 60);
	botubep();

	mau(250,250,250);
	beptu();


	xoong(0.2, 0, 0);
	xoong(-0.2, 0, 0);
	//cacloaihop(x, y, z);
}

void tudungdo() {
	mau(100, 120, 60);
	mattudungdo();
	mau(70, 120, 60);
	khungtudungdo();

	mau(100, 120, 60);
	hecanhtu();
}

void khuruabat() {
	mau(100, 120, 60);
	matkhuruabat(0, 0, -0.6);

	mau(230, 100, 100);
	bonruabat(-0.2, -0.4, -1);

	mau(70, 120, 60);
	khuturuabat();
	
	mau(100, 120, 60);
	hecanhturuabat();

	voirua(0, 0, 0);


}



void bep() {
	bep_m = Translate(-1.7,0.85,-3.7) * RotateY(180);
	khunaunuong();
	//mau(200, 100, 10);
	khuruabat();
	tudungdo();
}

////////////////////////////////////////////// BỘ BÀN GHẾ /////////////////////////////////////////////////////////////////////////////////////

mat4 model_ban, model_ghe;
GLfloat keoGhePhai, keoGheTrai;
void matBan()
{
	mat4 matban = Scale(1.5, 0.04, 2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ban * matban);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void chanBan(GLfloat n, GLfloat m)
{
	mat4 matban = Translate(n, -0.5, m) * Scale(0.04, 0.96, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ban * matban);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void matGhe(GLfloat b, GLfloat n, GLfloat m)
{
	mat4 matghe = Translate(b, n, m) * Scale(0.5, 0.04, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * matghe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lungGhe(GLfloat b, GLfloat n, GLfloat m)
{
	mat4 matghe = Translate(b, n, m) * Scale(0.04, 0.6, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * matghe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void chanGhe(GLfloat b, GLfloat n, GLfloat m)
{
	mat4 matghe = Translate(b, n, m) * Scale(0.04, 0.58, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * matghe);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void boBanGhe()
{
	
	
	//mau(139, 129 ,76);
	mau(139 ,69 ,0);
	model_ban = model * Translate(1, 1, 1);
	matBan();
	chanBan(0.73, 0.98);
	chanBan(-0.73, 0.98);
	chanBan(0.73, -0.98);
	chanBan(-0.73, -0.98);

	mau(139 ,105 ,20);
	model_ghe = model_ban * Translate(keoGhePhai, 0, 0);
	matGhe(1.1, -0.4, 0.7);
	lungGhe(1.33, -0.08, 0.7);
	chanGhe(0.87, -0.7, 0.93);
	chanGhe(1.33, -0.7, 0.93);
	chanGhe(0.87, -0.7, 0.47);
	chanGhe(1.33, -0.7, 0.47);

	matGhe(1.1, -0.4, 0);
	lungGhe(1.33, -0.08, 0);
	chanGhe(0.87, -0.7, 0.23);
	chanGhe(1.33, -0.7, 0.23);
	chanGhe(0.87, -0.7, -0.23);
	chanGhe(1.33, -0.7, -0.23);

	matGhe(1.1, -0.4, -0.7);
	lungGhe(1.33, -0.08, -0.7);
	chanGhe(0.87, -0.7, -0.47);
	chanGhe(1.33, -0.7, -0.47);
	chanGhe(0.87, -0.7, -0.93);
	chanGhe(1.33, -0.7, -0.93);

	model_ghe = model_ban * Translate(keoGheTrai, 0, 0);
	matGhe(-1.1, -0.4, 0.7);
	lungGhe(-1.33, -0.08, 0.7);
	chanGhe(-0.87, -0.7, 0.93);
	chanGhe(-1.33, -0.7, 0.93);
	chanGhe(-0.87, -0.7, 0.47);
	chanGhe(-1.33, -0.7, 0.47);

	matGhe(-1.1, -0.4, 0);
	lungGhe(-1.33, -0.08, 0);
	chanGhe(-0.87, -0.7, 0.23);
	chanGhe(-1.33, -0.7, 0.23);
	chanGhe(-0.87, -0.7, 0.27);
	chanGhe(-1.33, -0.7, 0.27);

	matGhe(-1.1, -0.4, -0.7);
	lungGhe(-1.33, -0.08, -0.7);
	chanGhe(-0.87, -0.7, -0.47);
	chanGhe(-1.33, -0.7, -0.47);
	chanGhe(-0.87, -0.7, -0.93);
	chanGhe(-1.33, -0.7, -0.93);

}

//////////////////////////////////////// TỦ LẠNH ///////////////////////////////////////////////////////////////////////

mat4 model_tu, model_hocTu, model_canhLanh;
GLfloat keoLanh, canhDuoi, canhTren;
void lanhSau()
{
	mat4 canh = Scale(1.2, 2, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lanhGiua(GLfloat a, GLfloat b)
{
	mat4 canh = Translate(0, a, 0.32) * Scale(1.12, b, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void lanhCanh(GLfloat a)
{
	mat4 canh = Translate(a, 0, 0.32) * Scale(0.04, 2, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void hocSau(GLfloat a)
{
	mat4 canh = Translate(0, -0.8, a) * Scale(1.12, 0.32, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_hocTu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hocDuoi()
{
	mat4 canh = Translate(0, -0.95, 0.3) * Scale(1.12, 0.02, 0.52);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_hocTu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hocCanh(GLfloat a)
{
	mat4 canh = Translate(a, -0.8, 0.3) * Scale(0.02, 0.32, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_hocTu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hocThanh()
{
	mat4 canh = Translate(0, -0.65, 0.58) * Scale(1.12, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_hocTu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hoc()
{
	model_hocTu = model_tu * Translate(0, 0, keoLanh);
	hocDuoi();
	hocSau(0.05);
	hocSau(0.55);
	hocCanh(0.55);
	hocCanh(-0.55);
	hocThanh();
}

void canhLanhDuoi()
{
	mat4 cua = Translate(1.18, 0, 0) * RotateY(canhDuoi) * Translate(-0.59, 0, 0) * Scale(1.18, 1.18, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_canhLanh * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhLanhTren()
{
	mat4 cua = Translate(1.18, 0.5, 0) * RotateY(canhTren) * Translate(-0.59, 0.5, 0) * Scale(1.18, 0.8, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_canhLanh * cua);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void khayDa()
{
	mat4 canh = Translate(1.18, 0.5, 0) * Scale(1.2, 2, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
mat4 model_dongho, model_donghog, model_donghop, model_donghoh;
GLfloat giay,phut=-60,gio=90;
void quayDongHo()
{
	giay -= 0.1;
	phut -= 0.1/60;
	gio -= 0.1 / (60 * 60);
	glutPostRedisplay();
}
void DongHo() {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dongho );
	drawCylinder(0.25, 0.02, 100); // Vẽ hình trụ tròn
}
void kimDongHo()
{
	mat4 canh = Translate(0.22, 0, 0.02) * Scale(0.06, 0.016, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dongho * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	canh = Translate(-0.22, 0, 0.02) * Scale(0.06, 0.016, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dongho * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	canh = Translate(0, 0.22, 0.02) * Scale(0.016, 0.06, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dongho * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	canh = Translate(0,-0.22, 0.02) * Scale(0.016, 0.06, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dongho * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void kimDongHoNho(GLfloat x,GLfloat y)
{
	mat4 canh = Translate(x, y, 0.02) * Scale(0.02, 0.02, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dongho * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void kimgiay()
{
	mat4 canh = Translate(0.08, 0, 0.02) * Scale(0.16, 0.01, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_donghog * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void kimphut()
{
	mat4 canh = Translate(0.075, 0, 0.02) * Scale(0.13, 0.013, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_donghop * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void kimgio()
{
	mat4 canh = Translate(0.05, 0, 0.02) * Scale(0.1, 0.016, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_donghoh * canh);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void tuLanh()
{
	mau(176, 224, 230); 

	model_tu = model * Translate(2, 1, -3.8);
	lanhSau();
	lanhCanh(-0.58);
	lanhCanh(0.58);
	lanhGiua(-0.98, 0.04);
	lanhGiua(0.2, 0.04);
	lanhGiua(0.98, 0.04);
	model_canhLanh = model_tu * Translate(-0.58, -0.4, 0.62);
	canhLanhDuoi();
	canhLanhTren();

	mau(254, 254, 254);
	lanhGiua(-0.2, 0.01);
	lanhGiua(-0.58, 0.01);
	lanhGiua(0.7, 0.02);
	hoc();

	model_dongho = model * Translate(2, 2.6, -3.98);
	DongHo();
	mau(255, 255, 255);
	kimDongHo();
	kimDongHoNho(0.2, 0.12);
	kimDongHoNho(0.12, 0.195);
	kimDongHoNho(-0.2, 0.12);
	kimDongHoNho(-0.12, 0.195);
	kimDongHoNho(-0.2, -0.12);
	kimDongHoNho(-0.12, -0.2);
	kimDongHoNho(0.2, -0.12);
	kimDongHoNho(0.12, -0.2);


	mau(255, 0, 0);
	model_donghog = model_dongho * RotateZ(giay);
	kimgiay();
	model_donghop = model_dongho * RotateZ(phut);
	kimphut();
	model_donghoh = model_dongho * RotateZ(gio);
	kimgio();
	quayDongHo();
	

	//khayDa();
}

///////////////////////////////////////// TỦ TRÊN ////////////////////////////////////////////////////////////////

mat4 model_cuaTuTren1,model_cuaTuTren2, model_cuaTuTren3, model_cuaTuTren4, model_mat;
GLfloat alp1,alp2,alp3,alp4;


void khungTu() {
	mat4 instance = Translate(1, -0.5, -3.3) * Scale(0.6, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(1, -0.5, -3.3+0.9) * Scale(0.6, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(1, -0.5, -3.3 + 0.9 + 0.9) * Scale(0.6, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(1, -0.5, -3.3 + 0.9 + 0.9 +0.9) * Scale(0.6, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(1, -0.5, -3.3 + 0.9 + 0.9 + 0.9 + 0.9) * Scale(0.6, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void matTren()
{
	mat4 instance = Translate(-4.1, 2, 1) * Scale(0.6, 0.04, 3.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_mat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void matDuoi()
{
	mat4 instance = Translate(-4.1, 1.4, 1) * Scale(0.6, 0.04, 3.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_mat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void cuaTuTren(GLfloat k,mat4 model_cuaTuTren)
{
	mat4 instance = Translate(0, -0.3, k) * Scale(0.04, 0.6, 0.85);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_cuaTuTren * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}

void hutMui()
{
	mat4 instance = Translate(-0,-0.73,0) * Scale(1, 0.16, 0.6);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void hutMui2()
{
	mat4 instance = Translate(-0, -0.61, 0) * Scale(0.8, 0.08, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void ongThoat() {
	mat4 instance =  Translate(0, 1.2, 0) * RotateX(90);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * bep_m * instance);
	drawCylinder(0.12, 2, 100);
}

void khungTuFull()
{
	khungTu();
	matTren();
	matDuoi();
}



void hutMuiFull()
{
	hutMui();
	hutMui2();
	ongThoat();
}

void tubatdua(){
	bep_m = Translate(-1.7, 2.8, -3.7) * RotateY(180);

	model_mat = model * Translate(2 - 0.58, 1-0.4, -3.8 +0.62);
	mau(70, 120, 60);
	khungTuFull();

	model_cuaTuTren1 = model * Translate(-2.5, 2.62, -1.26) * RotateZ(alp1) ;
	model_cuaTuTren2 = model * Translate(-2.5, 2.62, -1.26) * RotateZ(alp2) ;
	model_cuaTuTren3 = model * Translate(-2.5, 2.62, -1.26) * RotateZ(alp3) ;
	model_cuaTuTren4 = model * Translate(-2.5, 2.62, -1.26) * RotateZ(alp4) ;
	mau(100, 120, 60);
	cuaTuTren(0.4, model_cuaTuTren1);
	cuaTuTren(-0.5,model_cuaTuTren2);
	cuaTuTren(-1.4, model_cuaTuTren3);
	cuaTuTren(-2.3, model_cuaTuTren4);

	mau(100, 120, 60);
	hutMuiFull();

}

////////////////////////////////////// THUNG RAC /////////////////////////////////////

mat4 model_rac;
GLfloat alpha3, chanRac=0.2;


void khungThungRac()
{
	mat4 instance = Translate(-4.2, -0.3, 3.4) * Scale(0.35, 0.5, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_mat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-4.2, -0.3, 3) * Scale(0.35, 0.5, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_mat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance =  Translate(-4.35,  -0.3, 3.2) * Scale(0.04, 0.5, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_mat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-4.05, -0.3, 3.2) * Scale(0.04, 0.5, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_mat * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void napThung()
{
	mat4 instance = Translate(0.15, 0.08, 0) * Scale(0.38, 0.04, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_rac * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void dapChan()
{
	mat4 instance = Scale(0.1, 0.01, 0.15);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_rac * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void thungRac()
{
	mau(37, 190, 63);
	khungThungRac();
	model_rac = model * Translate(-2.89, 0.505, 0.01) * RotateZ(alpha3);
	napThung();
	model_rac = model * Translate(-2.58, chanRac, 0.01);
	dapChan();
}








GLfloat xxx, yyy, zzz; // di chuyển cam
GLfloat l = -1, r = 1, bottom = -1, top = 1, zNear = 1, zFar = 18;  // projection
GLfloat anpha, beta, gama; // góc xoay
void gocNhin()
{
	vec4 eye = vec4(0 + xxx, 1.5 + yyy, 8 + zzz, 1);
	vec4 at = vec4(0 + xxx, 0.5 + yyy, 3 + zzz, 1);
	vec4 up = vec4(0, 1, 0, 1);

	mat4 u = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, u);

	mat4 p = Frustum(l, r, bottom, top, zNear, zFar);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);

}

void display( void )
{

    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );   
	//mau(192, 192, 192);
	gocNhin();
	model = RotateX(anpha)*RotateY(beta)*RotateZ(gama);
	taoPhong();
	bep();
	boBanGhe();
	tuLanh();
	tubatdua();
	//glutWireSphere(1.0f, 20, 20);
	thungRac();
	glutSwapBuffers();									   
}



void keyboard( unsigned char key, int x, int y )
{
	switch (key) {
	case 033:
		exit(1);
		break;
		// di chuyển cam //
	case 'w':
		yyy++;
		break;
	case 's':
		yyy--;
		break;
	case 'a':
		xxx--;
		break;
	case 'd':
		xxx++;
		break;
		// tiến lùi cam //
	case 'U':
		zzz++;
		break;
	case 'u':
		zzz--;
		break;
		// xoay phòng //
	case 'i':
		anpha += 10;
		if (anpha > 360) anpha -= 360;
		break;
	case 'I':
		anpha -= 10;
		if (anpha < -360) anpha += 360;
		break;
	case 'o':
		beta += 10;
		if (beta > 360) beta -= 360;
		break;
	case 'O':
		beta -= 10;
		if (beta < -360) beta += 360;
		break;
	case 'p':
		gama += 10;
		if (gama > 360) gama -= 360;
		break;
	case 'P':
		gama -= 10;
		if (gama < -360) gama += 360;
		break;

	// ngan tu cua bep
	case '1':			
		if (alpha <= 20) {
			tubepxyz[0] -= 0.1;
			if (tubepxyz[0] <= -0.4) tubepxyz[0] = -0.4;
		}
		break;
	case '2':			
		tubepxyz[0] += 0.1;
		if (tubepxyz[0] >= 0) tubepxyz[0] = 0;
		
		break;
	case '3':	
		if (alpha <= 20)
		{
			tubepxyz[1] -= 0.1;
			if (tubepxyz[1] <= -0.4) tubepxyz[1] = -0.4;
		}
		
		break;
	case '4':			
		tubepxyz[1] += 0.1;
		if (tubepxyz[1] >= 0) tubepxyz[1] = 0;

		break;
	case '5':			
		if (alpha <= 20) {
			tubepxyz[2] -= 0.1;
			if (tubepxyz[2] <= -0.4) tubepxyz[2] = -0.4;
		}

		break;
	case '6':			
		tubepxyz[2] += 0.1;
		if (tubepxyz[2] >= 0) tubepxyz[2] = 0;

		break;
	case '7':			
		if (alpha <= 20) {
			tubepxyz[3] -= 0.1;
			if (tubepxyz[3] <= -0.4) tubepxyz[3] = -0.4;
		}

		break;
	case '8':			
		tubepxyz[3] += 0.1;
		if (tubepxyz[3] >= 0) tubepxyz[3] = 0;
		break;
		// mở tủ dưới //
	case 'q':		
		if (tubepxyz[0] == 0 && tubepxyz[1] == 0 && tubepxyz[2] == 0 && tubepxyz[3] == 0) {
			alpha += 10;
			if (alpha >= 90) alpha = 90;
		}
		break;
	case 'Q':			
		alpha -= 10;
		if (alpha <= 0) alpha = 0;
		break;
		// xoay vòi nước //
	case 'c':
		voirua_q += 10;
		if (voirua_q >= 30) voirua_q = 30;
		break;
	case 'C':
		voirua_q -= 10;
		if (voirua_q <= -30) voirua_q = -30;
		break;

		// mở các cánh tủ trên //
	case 'e':		
			alp1 += 10;
			if (alp1 >= 80) alp1 = 80;
		break;
	case 'E':
		alp1 -= 10;
		if (alp1 <= 0) alp1 = 0;
		break;
	case 'r':
		alp2 += 10;
		if (alp2 >= 80) alp2 = 80;
		break;
	case 'R':
		alp2 -= 10;
		if (alp2 <= 0) alp2 = 0;
		break;
	case 't':
		alp3 += 10;
		if (alp3 >= 80) alp3 = 80;
		break;
	case 'T':
		alp3 -= 10;
		if (alp3 <= 0) alp3 = 0;
		break;
	case 'y':
		alp4+= 10;
		if (alp4 >= 80) alp4 = 80;
		break;
	case 'Y':
		alp4 -= 10;
		if (alp4 <= 0) alp4 = 0;
		break;
		// mở thùng rác //
	case 'z':
		alpha3 += 10;
		chanRac -= 0.02;
		if (chanRac < 0.04) chanRac = 0.04;
		if (alpha3 >= 80) alpha3 = 80;
		break;
	case 'Z':
		alpha3 -= 10;
		chanRac += 0.015;
		if (chanRac >0.2) chanRac = 0.2;
		if (alpha3 <= 0) alpha3 = 0;
		break;


		// mở cửa chính //
	case 'j':
		goccua -= 10;
		if (goccua < -135) goccua =-135;
		break;
	case 'J':
		goccua += 10;
		if (goccua >0) goccua = 0;
		break;
		// mở cửa sổ //
	case 'k':
		gocso += 10;
		if (gocso >120) gocso = 120;
		break;
	case 'K':
		gocso -= 10;
		if (gocso < 0) gocso = 0;
		break;
		// bật quạt //
	case 'l':
		checked = !checked;  
		glutPostRedisplay();
		break;
	case 'L':
		quayQuat += 15;
		glutPostRedisplay();
		break;
		// kéo ghế //
	case 'm':
		keoGhePhai -= 0.05;
		if (keoGhePhai < -0.53) keoGhePhai = -0.53;
		keoGheTrai += 0.05;
		if (keoGheTrai > 0.53) keoGheTrai = 0.53;
		break;
	case 'M':
		keoGhePhai += 0.05;
		if (keoGhePhai > 0) keoGhePhai = 0;
		keoGheTrai -= 0.05;
		if (keoGheTrai < 0) keoGheTrai = 0;
		break;
		// kéo hốc tủ lạnh //
	case 'v':
		keoLanh += 0.03;
		if (keoLanh > 0.45) keoLanh = 0.5;
		break;
	case 'V':
		keoLanh -= 0.03;
		if (keoLanh < 0) keoLanh = 0;
		break;
		// mở cánh dưới tủ lạnh //
	case 'b':
		canhDuoi += 10;
		if (canhDuoi > 91) canhDuoi = 90;
		break;
	case 'B':
		canhDuoi -= 10;
		if (canhDuoi < 0) canhDuoi = 0;
		break;
		// mở cánh trên tủ lạnh //
	case 'n':
		canhTren += 10;
		if (canhTren > 91) canhTren = 90;
		break;
	case 'N':
		canhTren -= 10;
		if (canhTren < 0) canhTren = 0;
		break;
		//  bật đèn //
	case 'x':
		batDen = !batDen;
		if (!batDen) {
			choiSang = 0.1;
			mauAnhSang = 0.3;
		}
		else {
			choiSang = 0.6;
			mauAnhSang = 1;
		}
		break;
		// bật tivi //
	case 'f':
		batTV = !batTV;
		glutPostRedisplay();
		break;
	
	}
	glutPostRedisplay();
}


int main( int argc, char **argv )
{
	// main function: program starts here

    glutInit( &argc, argv );                       
    glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize( 640, 640 );                 
	glutInitWindowPosition(100,150);               
    glutCreateWindow( "Drawing a Cube" );            

   
	glewInit();										

    generateGeometry( );                          
    initGPUBuffers( );							   
    shaderSetup( );                               

    glutDisplayFunc( display );                   
    glutKeyboardFunc( keyboard );                  
	//glutReshapeFunc(reshape);

	glutMainLoop();
    return 0;
}
