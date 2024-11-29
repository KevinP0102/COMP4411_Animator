#include "modelerview.h"
#include "modelerapp.h"
#include "modelerui.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <FL/glu.h>
#include "vec.h"
#include "bitmap.h"

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

enum RobotCtrls {
	BODYHEIGHT, BODYWIDTH, NECKHEIGHT, HEADSCALE, 
	LEFTARM1X, LEFTARM1Y, LEFTARM1Z, LEFTARM2X, LEFTARM2Y, LEFTARM2Z,
	RIGHTARM1X, RIGHTARM1Y, RIGHTARM1Z, RIGHTARM2X, RIGHTARM2Y, RIGHTARM2Z,
	LEFTLEGX, LEFTLEGY, LEFTLEGZ, 
	RIGHTLEGX, RIGHTLEGY, RIGHTLEGZ,
	BOTHLEGSX, BOTHLEGSY, BOTHLEGSZ,
	HEADTYPE, HANDTYPE, FOOTTYPE,
	HEADCOLOR, HANDCOLOR, FOOTCOLOR,
	LSYSSHOW, LSYSLENGTH, LSYSANGLE, LSYSDEPTH, LSYSLENGTHREDUCTION,
	TOTAL
};

#define COLOR_WHITE 1.0f, 1.0f, 1.0f
#define COLOR_ORANGE 1.0f, 0.5f, 0.0f
#define COLOR_YELLOW 1.0f, 1.0f, 0.0f
#define COLOR_LIGHT_BLUE 0.0f, 1.0f, 1.0f
#define COLOR_GREY 0.5f, 0.5f, 0.5f

static GLfloat lightPosition2[] = { -20, -20, -20, 0 };
static GLfloat lightDiffuse2[] = { 1,1,1,0.5 };
static GLfloat lightAmbient2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
static GLint angleInc = 0;

class Robot : public ModelerView {
public:
	Robot(int x, int y, int w, int h, char* label) 
		: ModelerView(x, y, w, h, label) {}

	void drawPentagonalBipyramid();
	void drawSolidTorus(GLfloat innerRadius, GLfloat outerRadius, GLint sides, GLint rings);
	void drawLSystem(GLfloat len, GLfloat angle, GLint depth, GLfloat lenreduction );

	virtual void draw();
};

ModelerView* createRobot(int x, int y, int w, int h, char* label) {
	return new Robot(x, y, w, h, label);
}

void Robot::drawPentagonalBipyramid() {
	Vec3f top = Vec3f(0, 0, 1);
	Vec3f bottom = Vec3f(0, 0, -1);
	Vec3f square1 = Vec3f(1, 0, 0);
	Vec3f square2 = Vec3f(0, 1, 0);
	Vec3f square3 = Vec3f(-1, 0, 0);
	Vec3f square4 = Vec3f(0, -1, 0);

	drawTriangle(square1[0], square1[1], square1[2],
		square2[0], square2[1], square2[2],
		top[0], top[1], top[2]);
	drawTriangle(square2[0], square2[1], square2[2],
		square3[0], square3[1], square3[2],
		top[0], top[1], top[2]);
	drawTriangle(square3[0], square3[1], square3[2],
		square4[0], square4[1], square4[2],
		top[0], top[1], top[2]);
	drawTriangle(square4[0], square4[1], square4[2],
		square1[0], square1[1], square1[2],
		top[0], top[1], top[2]);

	drawTriangle(square1[0], square1[1], square1[2],
		square2[0], square2[1], square2[2],
		bottom[0], bottom[1], bottom[2]);
	drawTriangle(square2[0], square2[1], square2[2],
		square3[0], square3[1], square3[2],
		bottom[0], bottom[1], bottom[2]);
	drawTriangle(square3[0], square3[1], square3[2],
		square4[0], square4[1], square4[2],
		bottom[0], bottom[1], bottom[2]);
	drawTriangle(square4[0], square4[1], square4[2],
		square1[0], square1[1], square1[2],
		bottom[0], bottom[1], bottom[2]);

}

void Robot::drawLSystem(GLfloat len, GLfloat angle, GLint depth, GLfloat lenreduction) {
	if (depth == 0) {
		drawBox(0.1, 0.1, len);
	}
	else {
		glPushMatrix();
		drawBox(0.1, 0.1, len);
		

		glPushMatrix();
		glTranslated(0, 0, len);
		glRotated(angle, 1, 0, 0);
		drawLSystem(len * lenreduction, angle, depth - 1, lenreduction);
		glPopMatrix();

		glPushMatrix();
		glTranslated(0, 0, len);
		glRotated(-angle, 1, 0, 0);
		drawLSystem(len * lenreduction, angle, depth - 1, lenreduction);
		glPopMatrix();

		glPopMatrix();
	}

}

void Robot::drawSolidTorus(GLfloat innerRadius, GLfloat outerRadius, GLint sides, GLint rings) {
	GLfloat ringDelta = 2.0f * 3.141592653589793238462643383279502 / rings;
	GLfloat sideDelta = 2.0f * 3.141592653589793238462643383279502 / sides;
	GLfloat theta = 0.0f;
	GLfloat cosTheta = 1.0f;
	GLfloat sinTheta = 0.0f;

	for (int i = 0; i < rings; ++i) {
		GLfloat theta1 = theta + ringDelta;
		GLfloat cosTheta1 = cos(theta1);
		GLfloat sinTheta1 = sin(theta1);

		glBegin(GL_QUAD_STRIP);
		GLfloat phi = 0.0f;
		for (int j = 0; j <= sides; ++j) {
			phi += sideDelta;
			GLfloat cosPhi = cos(phi);
			GLfloat sinPhi = sin(phi);
			GLfloat dist = outerRadius + innerRadius * cosPhi;

			glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
			glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, innerRadius * sinPhi);
			glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
			glVertex3f(cosTheta * dist, -sinTheta * dist, innerRadius * sinPhi);
		}
		glEnd();

		theta = theta1;
		cosTheta = cosTheta1;
		sinTheta = sinTheta1;
	}
}

void Robot::draw()
{
	ModelerView::draw();

	glEnable(GL_LIGHT2);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient2);

	
	if (angleInc == 360) {
		angleInc = 0;
	}
	else {
		angleInc += 3;
	}

	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_GREEN);

	glPushMatrix();
		glTranslated(0, 0, 0);

		glPushMatrix();
		glTranslated(0, 1, 0);
		glRotated(90, 1, 0, 0);
		//body
		drawCylinder(VAL(BODYHEIGHT), VAL(BODYWIDTH), VAL(BODYWIDTH));

			//tail
			glPushMatrix();

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(.5f, .5f, .5f);
			glTranslated(0, -VAL(BODYWIDTH), VAL(BODYHEIGHT) / 2.0f);
			glRotated(90, 1, 0, 0);
			glRotated(angleInc, 0, 0, 1);
			drawCylinder(VAL(BODYWIDTH) * 0.25, 0.075 * VAL(BODYWIDTH), 0.075 * VAL(BODYWIDTH));

			glPushMatrix();
			glTranslated(-VAL(BODYWIDTH) * 0.075, -VAL(BODYWIDTH) * 0.25, VAL(BODYWIDTH) * 0.25);
			drawBox(0.15 * VAL(BODYWIDTH), VAL(BODYWIDTH) * 0.5, VAL(BODYWIDTH) * 0.2);
			glPopMatrix();

			glPopMatrix();
			
		//left arm 1
			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_RED);
			glPushMatrix();
			glTranslated(VAL(BODYWIDTH), 0, 0.2 * VAL(BODYHEIGHT));
			drawSphere(0.075 * VAL(BODYHEIGHT));

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_WHITE);
			glRotated(30, 0, 1, 0);
			glRotated(VAL(LEFTARM1X), 1, 0, 0);
			glRotated(VAL(LEFTARM1Y), 0, 1, 0);
			glRotated(VAL(LEFTARM1Z), 0, 0, 1);
			drawCylinder(0.4 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT));
			//left arm 2

				glPushMatrix();
				glTranslated(0, 0, 0.4 * VAL(BODYHEIGHT));
				setAmbientColor(.1f, .1f, .1f);
				setDiffuseColor(COLOR_RED);
				drawSphere(0.075 * VAL(BODYHEIGHT));

				setAmbientColor(.1f, .1f, .1f);
				setDiffuseColor(COLOR_WHITE);
				glRotated(-30, 0, 1, 0);
				glRotated(VAL(LEFTARM2X), 1, 0, 0);
				glRotated(VAL(LEFTARM2Y), 0, 1, 0);
				glRotated(VAL(LEFTARM2Z), 0, 0, 1);
				drawCylinder(0.4 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT));
				//left hand
				if (VAL(HANDTYPE) == 0) {
					glPushMatrix();
					glTranslated(0, 0.06 * VAL(BODYHEIGHT), 0.4 * VAL(BODYHEIGHT));

					
					if (VAL(HANDCOLOR) == 0)
						setDiffuseColor(COLOR_ORANGE);
					else if (VAL(HANDCOLOR) == 1)
						setDiffuseColor(COLOR_LIGHT_BLUE);
					
					glRotated(90, 1, 0, 0);
					drawCylinder(0.12 * VAL(BODYHEIGHT), 0.09 * VAL(BODYHEIGHT), 0.09 * VAL(BODYHEIGHT));

					glPopMatrix();
				}
				else
				{
					glPushMatrix();
					glTranslated(-0.075 * VAL(BODYHEIGHT), -0.075*VAL(BODYHEIGHT), (0.4 - 0.075) * VAL(BODYHEIGHT));

					setAmbientColor(.1f, .1f, .1f);
					if (VAL(HANDCOLOR) == 0)
						setDiffuseColor(COLOR_ORANGE);
					else if (VAL(HANDCOLOR) == 1)
						setDiffuseColor(COLOR_LIGHT_BLUE);

					glScaled(0.15 * VAL(BODYHEIGHT), 0.15 * VAL(BODYHEIGHT), 0.15 * VAL(BODYHEIGHT));
					drawBox(1, 1, 1);

					glPopMatrix();
				}

				if (VAL(LSYSSHOW)) {
					setDiffuseColor(COLOR_GREEN);
					glPushMatrix();
					glTranslated(0, 0, 0.4 * VAL(BODYHEIGHT));
					glRotated(-90, 1, 0, 0);
					drawLSystem(VAL(LSYSLENGTH), VAL(LSYSANGLE), VAL(LSYSDEPTH), VAL(LSYSLENGTHREDUCTION));
					glPopMatrix();
				}

				glPopMatrix();

			glPopMatrix();

			//right arm 1
			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_RED);
			glPushMatrix();
			glTranslated(-VAL(BODYWIDTH), 0, 0.2 * VAL(BODYHEIGHT));
			drawSphere(0.075 * VAL(BODYHEIGHT));

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_WHITE);
			glRotated(-30, 0, 1, 0);
			glRotated(VAL(RIGHTARM1X), 1, 0, 0);
			glRotated(VAL(RIGHTARM1Y), 0, 1, 0);
			glRotated(VAL(RIGHTARM1Z), 0, 0, 1);
			drawCylinder(0.4 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT));
			//right arm 2
				glPushMatrix();
				glTranslated(0, 0, 0.4 * VAL(BODYHEIGHT));
				setAmbientColor(.1f, .1f, .1f);
				setDiffuseColor(COLOR_RED);
				drawSphere(0.075 * VAL(BODYHEIGHT));

				setAmbientColor(.1f, .1f, .1f);
				setDiffuseColor(COLOR_WHITE);
				glRotated(30, 0, 1, 0);
				glRotated(VAL(RIGHTARM2X), 1, 0, 0);
				glRotated(VAL(RIGHTARM2Y), 0, 1, 0);
				glRotated(VAL(RIGHTARM2Z), 0, 0, 1);
				drawCylinder(0.4 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT));

			//right hand
				if (VAL(HANDTYPE) == 0) {
					glPushMatrix();
					glTranslated(0, 0.06 * VAL(BODYHEIGHT), 0.4 * VAL(BODYHEIGHT));


					if (VAL(HANDCOLOR) == 0)
						setDiffuseColor(COLOR_ORANGE);
					else if (VAL(HANDCOLOR) == 1)
						setDiffuseColor(COLOR_LIGHT_BLUE);

					glRotated(90, 1, 0, 0);
					drawCylinder(0.12 * VAL(BODYHEIGHT), 0.09 * VAL(BODYHEIGHT), 0.09 * VAL(BODYHEIGHT));

					glPopMatrix();
				}
				else
				{
					glPushMatrix();
					glTranslated(-0.075 * VAL(BODYHEIGHT), -0.075 * VAL(BODYHEIGHT), (0.4 - 0.075) * VAL(BODYHEIGHT));

					setAmbientColor(.1f, .1f, .1f);
					if (VAL(HANDCOLOR) == 0)
						setDiffuseColor(COLOR_ORANGE);
					else if (VAL(HANDCOLOR) == 1)
						setDiffuseColor(COLOR_LIGHT_BLUE);

					glScaled(0.15 * VAL(BODYHEIGHT), 0.15 * VAL(BODYHEIGHT), 0.15 * VAL(BODYHEIGHT));
					drawBox(1, 1, 1);

					glPopMatrix();
				}


				glPopMatrix();

			glPopMatrix();

			if (VAL(HEADCOLOR) == 0) {
				setDiffuseColor(COLOR_YELLOW);
			}
			else {
				setDiffuseColor(COLOR_ORANGE);
			}
			//neck
			glPushMatrix();
			glRotated(180, 1, 0, 0);
			drawCylinder(VAL(NECKHEIGHT), 0.2 * VAL(BODYWIDTH), 0.2 * VAL(BODYWIDTH));


			//head
			if (VAL(HEADTYPE) == 0) {
				glPushMatrix();
				glTranslated(-VAL(HEADSCALE) / 2.0, -VAL(HEADSCALE) / 4.0, VAL(NECKHEIGHT));
				glScaled(VAL(HEADSCALE), VAL(HEADSCALE) / 2.0, VAL(HEADSCALE) / 2.0);
				drawBox(1, 1, 1);
				glPopMatrix();
			}
			else {
				glPushMatrix();
				glTranslated(0, 0, VAL(NECKHEIGHT));

				drawCylinder(VAL(HEADSCALE) / 2.0, VAL(HEADSCALE) / 2.0, VAL(HEADSCALE) / 2.0);

				glPopMatrix();
			}
				
				setDiffuseColor(COLOR_RED);
				glPushMatrix();
				glTranslated(0, 0, VAL(NECKHEIGHT) + VAL(HEADSCALE));
				glScaled(0.5, 0.5, 0.5);
				drawPentagonalBipyramid();
				glPopMatrix();
				
				glPushMatrix();

				setDiffuseColor(COLOR_WHITE);
				glTranslated(0, 0, VAL(NECKHEIGHT) + VAL(HEADSCALE)-0.5);
				drawSolidTorus(0.1, 0.5, 10, 30);
				

				glPopMatrix();
				
			glPopMatrix();

			//left leg
			glPushMatrix();
			glTranslated(VAL(BODYWIDTH)/2.0, 0, VAL(BODYHEIGHT));

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_RED);
			drawSphere(0.075 * VAL(BODYHEIGHT));

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_WHITE);
			glRotated(VAL(LEFTLEGX) + VAL(BOTHLEGSX), 1, 0, 0);
			glRotated(VAL(LEFTLEGY) + VAL(BOTHLEGSY), 0, 1, 0);
			glRotated(VAL(LEFTLEGZ) + VAL(BOTHLEGSZ), 0, 0, 1);
			drawCylinder(0.4 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT));
			//left foot
			setAmbientColor(.1f, .1f, .1f);
			if (VAL(FOOTCOLOR) == 0)
				setDiffuseColor(COLOR_GREEN);
			else if (VAL(FOOTCOLOR) == 1)
				setDiffuseColor(COLOR_ORANGE);
			if (VAL(FOOTTYPE) == 0) {
				glPushMatrix();
				glTranslated(-0.1 * VAL(BODYHEIGHT), -0.1 * VAL(BODYHEIGHT), 0.4 * VAL(BODYHEIGHT));

				drawBox(0.2 * VAL(BODYHEIGHT), 0.2 * VAL(BODYHEIGHT), 0.075 * VAL(BODYHEIGHT));
				glPopMatrix();
			}
			else {
				glPushMatrix();
				glTranslated(0, 0, 0.4 * VAL(BODYHEIGHT));

				drawCylinder(0.075 * VAL(BODYHEIGHT), 0.1 * VAL(BODYHEIGHT), 0.1 * VAL(BODYHEIGHT));
				glPopMatrix();
			}
			
			glPopMatrix();

			//right leg
			glPushMatrix();
			glTranslated(-VAL(BODYWIDTH) / 2.0, 0, VAL(BODYHEIGHT));

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_RED);
			drawSphere(0.075 * VAL(BODYHEIGHT));

			setAmbientColor(.1f, .1f, .1f);
			setDiffuseColor(COLOR_WHITE);
			glRotated(VAL(RIGHTLEGX) + VAL(BOTHLEGSX), 1, 0, 0);
			glRotated(VAL(RIGHTLEGY) + VAL(BOTHLEGSY), 0, 1, 0);
			glRotated(VAL(RIGHTLEGZ) + VAL(BOTHLEGSZ), 0, 0, 1);
			drawCylinder(0.4 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT), 0.05 * VAL(BODYHEIGHT));

			//right foot
			setAmbientColor(.1f, .1f, .1f);
			if (VAL(FOOTCOLOR) == 0)
				setDiffuseColor(COLOR_GREEN);
			else if (VAL(FOOTCOLOR) == 1)
				setDiffuseColor(COLOR_ORANGE);
			if (VAL(FOOTTYPE) == 0) {
				glPushMatrix();
				glTranslated(-0.1 * VAL(BODYHEIGHT), -0.1 * VAL(BODYHEIGHT), 0.4 * VAL(BODYHEIGHT));

				drawBox(0.2 * VAL(BODYHEIGHT), 0.2 * VAL(BODYHEIGHT), 0.075 * VAL(BODYHEIGHT));
				glPopMatrix();
			}
			else {
				glPushMatrix();
				glTranslated(0, 0, 0.4 * VAL(BODYHEIGHT));
				
				drawCylinder(0.075 * VAL(BODYHEIGHT), 0.1 * VAL(BODYHEIGHT), 0.1 * VAL(BODYHEIGHT));
				glPopMatrix();
			}

			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
}

int main() {
	ModelerControl controls[TOTAL];
	controls[BODYHEIGHT] = ModelerControl("Body Height", 1.5f, 4.5f, 0.1f, 3.0f);
	controls[BODYWIDTH] = ModelerControl("Body Width", 1.0f, 2.0f, 0.1f, 1.3f);
	controls[NECKHEIGHT] = ModelerControl("Neck Height", 0.1f, 0.5f, 0.1f, 0.3f);
	controls[HEADSCALE] = ModelerControl("Head Scale", 1.5f, 3.0f, 0.1f, 2.0f);
	controls[LEFTARM1X] = ModelerControl("Left Arm 1 X", -180, 180, 1, 0);
	controls[LEFTARM1Y] = ModelerControl("Left Arm 1 Y", -180, 180, 1, 0);
	controls[LEFTARM1Z] = ModelerControl("Left Arm 1 Z", -180, 180, 1, 0);
	controls[LEFTARM2X] = ModelerControl("Left Arm 2 X", -180, 180, 1, 0);
	controls[LEFTARM2Y] = ModelerControl("Left Arm 2 Y", -180, 180, 1, 0);
	controls[LEFTARM2Z] = ModelerControl("Left Arm 2 Z", -180, 180, 1, 0);
	controls[RIGHTARM1X] = ModelerControl("Right Arm 1 X", -180, 180, 1, 0);
	controls[RIGHTARM1Y] = ModelerControl("Right Arm 1 Y", -180, 180, 1, 0);
	controls[RIGHTARM1Z] = ModelerControl("Right Arm 1 Z", -180, 180, 1, 0);
	controls[RIGHTARM2X] = ModelerControl("Right Arm 2 X", -180, 180, 1, 0);
	controls[RIGHTARM2Y] = ModelerControl("Right Arm 2 Y", -180, 180, 1, 0);
	controls[RIGHTARM2Z] = ModelerControl("Right Arm 2 Z", -180, 180, 1, 0);
	controls[LEFTLEGX] = ModelerControl("Left Leg X", -180, 180, 1, 0);
	controls[LEFTLEGY] = ModelerControl("Left Leg Y", -180, 180, 1, 0);
	controls[LEFTLEGZ] = ModelerControl("Left Leg Z", -180, 180, 1, 0);
	controls[RIGHTLEGX] = ModelerControl("Right Leg X", -180, 180, 1, 0);
	controls[RIGHTLEGY] = ModelerControl("Right Leg Y", -180, 180, 1, 0);
	controls[RIGHTLEGZ] = ModelerControl("Right Leg Z", -180, 180, 1, 0);
	controls[BOTHLEGSX] = ModelerControl("Both Legs X", -180, 180, 1, 0);
	controls[BOTHLEGSY] = ModelerControl("Both Legs Y", -180, 180, 1, 0);
	controls[BOTHLEGSZ] = ModelerControl("Both Legs Z", -180, 180, 1, 0);
	controls[HEADTYPE] = ModelerControl("Head Type", 0, 1, 1, 0);
	controls[HANDTYPE] = ModelerControl("Hand Type", 0, 1, 1, 0);
	controls[FOOTTYPE] = ModelerControl("Foot Type", 0, 1, 1, 0);
	controls[HEADCOLOR] = ModelerControl("Head Color", 0, 1, 1, 0);
	controls[HANDCOLOR] = ModelerControl("Hand Color", 0, 1, 1, 0);
	controls[FOOTCOLOR] = ModelerControl("Foot Color", 0, 1, 1, 0);
	controls[LSYSSHOW] = ModelerControl("L-System Show", 0, 1, 1, 0);
	controls[LSYSLENGTH] = ModelerControl("L-System Length", 0.1, 1, 0.1, 0.5);
	controls[LSYSANGLE] = ModelerControl("L-System Angle", 1, 15, 1, 5);
	controls[LSYSDEPTH] = ModelerControl("L-System Depth", 1, 5, 1, 3);
	controls[LSYSLENGTHREDUCTION] = ModelerControl("L-System Length Reduction", 0.5, 1, 0.1, 0.8);

	ModelerApplication::Instance()->Init(&createRobot, controls, TOTAL);

	return ModelerApplication::Instance()->Run();
}