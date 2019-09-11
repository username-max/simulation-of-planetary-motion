#include <iostream> 	
#include <cstdlib> 
#include <ctime> 
#include <vector> 
#include <cmath> 
#include "Your adresses"//connection glut.h

struct Vector2 //Структура векторных взаимодействий
{
float x, y;
Vector2(){}
Vector2(float _x, float _y) : x(_x), y(_y){}
Vector2(int _x, int _y) : x((float)_x), y((float)_y){}
Vector2& operator - ();
Vector2& operator += (const Vector2&);
Vector2& operator -= (const Vector2&);
Vector2& operator * (const Vector2&);
};

struct Color{ //Задаем цвет объектов в rgp коде
float r, g, b;
Color(){}
Color(float _r, float _g, float _b) : r(_r / 255), g(_g / 255), b(_b / 255){}
}

Vector2 & Vector2::operator-(){		//Конструктор присвоения отрицатель-ного значния
x = -x;
y = -y;
return *this;
}

Vector2& Vector2::operator += (const Vector2& vector){		//Конструктор сложения
x += vector.x;
y += vector.y;
return *this;
}

Vector2& Vector2::operator -= (const Vector2& vector){		//Конструктор вычитания
x -= vector.x;
y += vector.y;
return *this;
}

Vector2& Vector2::operator * (const Vector2& vector){		//Конструктор умножения
x *= vector.x;
y *= vector.y;
return *this;
}

class CelestialObject{		//Класс небесное тело
Vector2 position;			//Позиция на экране объекта
Vector2 acceleration;		//Ускорение объекта
Color color;
int r, m;
public:
CelestialObject() : position(Vector2(0, 0)), acceleration(Vec-tor2(0, 0)), r(10), m(500), color(Color(0, 0, 0))//Конструктор
{}
CelestialObject(Vector2 _pos, Vector2 _vel, int _r, int _m, Color _color) : position(_pos), acceleration(_vel), r(_r), m(_m), color(_color)//Прототип функции
{}
void Draw();
void addedPosition(Vector2);
void addedAcceleration(Vector2);
void addRadius(float);
void addMass(float);
Vector2 getPosition();
Vector2 getAcceleration();
int getRadius();
int getMass();
};

void CelestialObject::Draw(){				//Создание объекта
glPushMatrix(); 							//Обращение в матрицу
glTranslatef(position.x, position.y, 0 );	//Задаем позицию объ-екта на экране
GLfloat theta;
GLfloat pi = acos(-1.0);					//Половина круга
GLfloat step = 0.1f; 						// чем больше шаг тем меньше разрешение планеты, но меньше нагрузка на процессор
											// рисуем диск по часовой стрелки
glBegin(GL_TRIANGLE_FAN);					//Функция нужна для рисования объекта
for (GLfloat a = 0.0f; a < 360.0f; a += step) {
	theta = 2.0f * pi * a / 180.0f;		 		 	//Создаем коэффициент
	glColor3f(color.r, color.g, color.b);			//задаем ее цвет
	glVertex3f(r* cos(theta), r* sin(theta), 0.0f);	//Рисуем небесное тело
}
glEnd();				//Закончили рисовать
glPopMatrix();			//сохранение текущих координат объекта
}

void CelestialObject::addedPosition(Vector2 vector){
position += vector;
}

void CelestialObject::addedAcceleration(Vector2 vector){
acceleration += vector;
}

Vector2 CelestialObject::getPosition(){
return position;
}

Vector2 CelestialObject::getAcceleration(){
return acceleration;
}

int CelestialObject::getRadius(){
return r;
}

int CelestialObject::getMass(){
return m;
}

class Display{
	int width, height, speed;
	//Масштаб для окна
	float thousandsKilometersPixel;
	public:
	Display(int width, int height, int thousandsKilometersPixel, int speed):
	width(width),
	height(height),
	thousandsKilometersPixel(thousandsKilometersPixel),
	speed(speed)
{
if (thousandsKilometersPixel < 1) thousandsKilometersPixel = 1;
}
int getWidth();
int getHeight();
float getScale();
float getSpeed();
};

int Display::getWidth(){
return width;
}

int Display::getHeight(){
return height;
}

float Display::getScale(){
return thousandsKilometersPixel;
}

float Display::getSpeed(){
return speed;
}

class Space{					//Класс взаимоействия небесных тел
	std::vector<CelestialObject*> objectsArray;//Вектор класса Небесное тело
	float ForceOfGravity( CelestialObject*, Celes-tialObject*);//Метод гравитационных взаимодействий
	public:
	Space()
	{}
	void add(CelestialObject);
	void accelerationObjects();
	void moveObjects();
	void DrawingAllObjects();
};

float Space::ForceOfGravity( CelestialObject* o1, CelestialObject* o2){
	float x = 0, y = 0;
	x = std::abs(o1->getPosition().x - o2->getPosi-tion().x);//Векторное
	y = std::abs(o1->getPosition().y - o2->getPosi-tion().y);//сложение
	float r = x + y;
	return (o1->getMass() * o2->getMass() / pow(r, 2)) / 500000;//Закон всемирного тяготения
}
void Space::add(CelestialObject object){
	objectsArray.push_back(new CelestialObject(ob-ject));//Записываем созданный объект в вектор
}
void Space::accelerationObjects(){ 					//Функция ускорения
	int countPlanets = (int)objectsArray.size();	//Размер век-тора
	int *cP = &countPlanets;						//Указатель на размер вектора
	for (int i = 0; i < *cP; i++){					//Идем по вектору
		for (int j = 0; j < *cP && (i != j); j++){
			float fG = ForceOfGravity( objectsArray[i], objectsAr-ray[j]);
			float massJtoI = ((float)objectsArray.at(j)->getMass() / objectsArray.at(i)->getMass());
			float massItoJ = ((float)objectsArray.at(i)->getMass() / objectsArray.at(j)->getMass());
			float fGmassJtoI = fG * massJtoI;
			float fGmassItoJ = fG * massItoJ;
			Vector2 direction 				//Направление
			(
				objectsArray.at(i)->getPosition().x > objectsAr-ray.at(j)->getPosition().x? -1 : 1,
				objectsArray.at(i)->getPosition().y > objectsAr-ray.at(j)->getPosition().y? -1 : 1
			);
			objectsArray.at(i)->addedAcceleration(Vector2(fGmass-JtoI, fGmassJtoI) * direction);
			objectsArray.at(j)->addedAcceleration(Vec-tor2(fGmassItoJ, fGmassItoJ) * -direction);
		}
	}
}
void Space::moveObjects(){				//Функция движения объектов по окну
	for (CelestialObject* object : objectsArray){
		object->addedPosition(object->getAcceleration());
	}
}
void Space::DrawingAllObjects(){		//Рисуем наши объекты
	for (CelestialObject* object : objectsArray){
		object->Draw();
	}
}
Space gravity;
Display display(683, 384, 3, 1);		//Ставим размеры окна, данный раз-мер зависит от расширения вашего экрана
void Start(){
	glClearColor(0, 0, 0, 0);			// Задаем цвет фона окна в черный
	glMatrixMode(GL_PROJECTION); 		// говорит о том, что следующие параметры будут относиться именно к нашему проэкту
	gluOrtho2D(-display.getWidth() * display.getScale(), dis-play.getWidth() * display.getScale(), -display.getHeight() * dis-play.getScale(), display.getHeight() * display.getScale());
}
void Draw(){
	glClear(GL_COLOR_BUFFER_BIT);		//Очистка окна
	gravity.DrawingAllObjects();
	glutSwapBuffers(); 					//Переставление буфера
}
void Update(int){
	glutPostRedisplay(); 				//Перерисовка текущего окна
	glutTimerFunc(display.getSpeed(), Update, 0); 		//Эта функция устанавливает таймер.
	gravity.accelerationObjects(); 		//Передаем параметы движения
	gravity.moveObjects(); 				//и ускорения
}

int main(int argc, char * argv[]){
	gravity.add(CelestialObject(Vector2(0, 0), Vector2(0, 0), 40, 10000, Color(240, 211, 139)));		//Планета
	gravity.add(CelestialObject(Vector2(250, 0), Vector2(0.0f, 1.0f), 10, 100, Color(125, 99, 34)));	//Спутник1
	gravity.add(CelestialObject(Vector2(270, 0), Vector2(0.0f, 1.0f), 10, 100, Color(119, 173, 52)));	//Спутник2
	gravity.add(CelestialObject(Vector2(320, 0), Vector2(0.0f, 1.0f), 10, 100, Color(138, 143, 131)));	//Спутник3
	gravity.add(CelestialObject(Vector2(370, 0), Vector2(0.0f, 0.8f), 10, 100, Color(194, 87, 0)));		//Спутник4
	glutInit(&argc, argv); 																				//Инициализация GLUT
	glutInitDisplayMode(GLUT_DOUBLE); 																	//GLUT_DOUBLE Вывод в окно осуществляется с использованием 2 буферов. Применяется для анима-ции, чтобы исключить эффект мерцания.
	glutInitWindowSize(display.getWidth(), display.getHeight()); 										//Установка параметров окна
	glutInitWindowPosition(300, 200); 																	//Положение создаваемого окна относительно верхнего левого угла экрана
	glutCreateWindow("Gravity"); 																		//Создание окна с именем "Gravity"
	Start();
	glutDisplayFunc(Draw);																				//Функция рисование в окне
	glutTimerFunc(2, Update, 0);																		//Обновление времени, чтобы тела наши двигались
	glutMainLoop();
	return 0;
}