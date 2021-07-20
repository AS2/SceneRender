#version 400 core
out vec4 FragColor;
#define MAX_OBJECTS_SIZE 100 // copied from core.h
#define RAY_INF 1000          // copied from render_cpu_rt
#define RAY_SHIFT 0.001

struct Object
{
	int type;
	double arr[16];
};

// for example lets pass size
uniform vec2 WindowSize;
uniform vec3 backgroundColor;


layout (shared) uniform Objects_Block
{
	Object Objects[MAX_OBJECTS_SIZE];
};


uniform int Objects_size;

struct ray
{
	vec3 org;
	vec3 dir;
};

// type enum
#define SPHERE 0
#define RECTANGLE 1
#define PLANE 2
#define BOX 3
#define TRIANGLE 4
#define NUM_OF_OBJECT_TYPES 5

vec3 ray_cast_sphere(Object obj, ray Ray)
{
	double Radius = obj.arr[3];
	vec3 Center = vec3(obj.arr[0],obj.arr[1], obj.arr[2]);

	vec3 l = Center - Ray.org;
	double t1 = dot(l, Ray.dir);

	if (t1 <= 0)
		return backgroundColor;
	else 
	{
		double d = dot(l, l) - t1 * t1;
		if (d > Radius * Radius)
			return backgroundColor;

		double t2 = sqrt(Radius * Radius - d), t = t1 - t2;
		if (t > RAY_INF)
			return backgroundColor;

		vec3 cD = Ray.org + vec3(Ray.dir * t);
		//pos = sph->Pos + (cD - sph->Pos) * (1 + RAY_SHIFT);
		return vec3(1, 0, 0);
	}
}

vec3 ray_cast_object( Object obj, ray Ray)
{
	if (obj.type == SPHERE)
	{
		return ray_cast_sphere(obj, Ray);
	}
	else
	{
		return backgroundColor;
	}
}

void main()
{
	ray screen_ray;
	vec3 cam = vec3(5.5, 0, 0);
	double scrCoordW = 5.5,
             scrCoordH = 5.5 * WindowSize.y / WindowSize.x,
             pixelSize = scrCoordW / ( WindowSize.x * 1);
	 double xCoord = 0,
		yCoord = 0 - scrCoordW / 2 + pixelSize * (gl_FragCoord.x + 1.0 / 2),
		zCoord = 0 + scrCoordH / 2 - pixelSize * (gl_FragCoord.y + 1.0 / 2);
	
	vec3 screenPointCrd = vec3(xCoord, yCoord, zCoord);
    screen_ray.org = screenPointCrd;
	screen_ray.dir = normalize(screenPointCrd - cam);

    for (int i = 0; i < Objects_size; i++)
	{
		vec3 rez = ray_cast_object(Objects[i], screen_ray);
		if (rez != backgroundColor)
			FragColor = vec4(rez, 1);
	}
	//FragColor = vec4(backgroundColor, 1);
    //FragColor = vec4(screenPointCrd.y, screenPointCrd.z, 0.2f, 1.0f);
}