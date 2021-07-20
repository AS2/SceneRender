#version 400 core
out vec4 FragColor;
#define MAX_OBJECTS_SIZE 128 // copied from core.h
#define RAY_INF 1000          // copied from render_cpu_rt
#define RAY_SHIFT 0.001


// for example lets pass size
uniform vec2 WindowSize;
uniform vec3 backgroundColor;

struct Object
{
	vec4 arr1;
	vec4 arr2;
	vec4 arr3;
	vec3 arr4;
	int type;
};

struct Material
{
	vec4 arr1;
	vec4 arr2;
};

layout (std140) uniform Objects_Block
{
	Object Objects[MAX_OBJECTS_SIZE];
};

layout (std140) uniform Materials_Block
{
	Material Materials[MAX_OBJECTS_SIZE];
};


uniform int Objects_size;

struct ray
{
	vec3 org;
	vec3 dir;
};

struct returnCollision 
{
	int collRes;
	vec3 color;
	vec3 pos;
};

// type enum
#define SPHERE 0
#define RECTANGLE 1
#define PLANE 2
#define BOX 3
#define TRIANGLE 4
#define NUM_OF_OBJECT_TYPES 5

vec3 normalVec3(vec3 v)
{
	double length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	if (length != 0)
		return vec3(v.x / length, v.y / length, v.z / length);
	else
		return vec3(0, 0, 0);
}

returnCollision ray_cast_sphere(Object obj, ray Ray, int objIndex)
{
	returnCollision rtrn;
	rtrn.collRes = 0;
	rtrn.color = backgroundColor;

	vec3 Center = obj.arr1.xyz;
	float Radius = obj.arr1.w;
	
	vec3 l = Center - Ray.org;
	double t1 = dot(l, Ray.dir);
	if (t1 <= 0)
		return rtrn;
	else 
	{
		double d = dot(l, l) - t1 * t1;
		if (d > Radius * Radius)
			return rtrn;

		double t2 = sqrt(Radius * Radius - d), t = t1 - t2;
		if (t > RAY_INF)
			return rtrn;
			
		vec3 cD = Ray.org + vec3(Ray.dir * t);
		rtrn.pos = Center + (cD - Center) * (1 + RAY_SHIFT);
		rtrn.collRes = 1;

		// ≈—À» “ŒÀ‹ Œ –¿—— ŒÃ≈Õ“»–Œ¬¿“‹ —“–Œ◊ ” Õ»∆≈, “Œ ¬—≈ œŒ…ƒ≈“ œŒ ... » œ≈–≈—“¿Õ≈“ ¬€¬Œƒ»“‹—ﬂ
		//rtrn.color = vec3(Materials[objIndex].arr1.x, Materials[objIndex].arr1.y, Materials[objIndex].arr1.z);

		float newRed = float(objIndex + 2) / Objects_size;
		rtrn.color = vec3(newRed, 0, 0);
		return rtrn;
	}
}

returnCollision NormalRectCheckCollision(ray Ray, vec3 LDcorn, vec3 RDcorn, vec3 LUcorn, vec3 normal, int objIndex)
{
	returnCollision rtrn;
	rtrn.collRes = 0;
	rtrn.color = backgroundColor;

	// find collision dot with plane
	if (dot(Ray.dir, normal) == 0)
		return rtrn;

	float d = (-1) * dot(normal, LDcorn),
		t = -(dot(Ray.org, normal) + d) / dot(Ray.dir, normal);

	if (t <= 0 || t > RAY_INF)
		return rtrn;

	// check if we are in plane piece
	vec3 dotOnPlane = Ray.org + Ray.dir * t,
		newVec = (dotOnPlane - LDcorn), v1 = (RDcorn - LDcorn), v2 = (LUcorn - LDcorn),
		xPart = normalVec3(v2) * length(newVec) * dot(newVec, v2) / (length(newVec) * length(v2)),
		yPart = normalVec3(v1) * length(newVec) * dot(newVec, v1) / (length(newVec) * length(v1));

	double alpha, beta;

	if (v2.x != 0)
		alpha = xPart.x / v2.x;
	else if (v2.y != 0)
		alpha = xPart.y / v2.y;
	else
		alpha = xPart.z / v2.z;

	if (v1.x != 0)
		beta = yPart.x / v1.x;
	else if (v1.y != 0)
		beta = yPart.y / v1.y;
	else
		beta = yPart.z / v1.z;

	if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1) {
		rtrn.pos = Ray.org + Ray.dir * (t - RAY_SHIFT);
		if (dot((rtrn.pos - LDcorn), (Ray.org - LDcorn)) < 0)
			rtrn.pos = Ray.org + Ray.dir * (t + RAY_SHIFT);

		rtrn.collRes = 1;
		//rtrn.color = Materials[objIndex].arr1.xyz;
		rtrn.color = vec3(0, 1, 0);
		return rtrn;
	}
	else
		return rtrn;
}

returnCollision ray_cast_rectangle(Object obj, ray Ray, int objIndex)
{
	returnCollision rtrn;
	rtrn.collRes = 0;
	rtrn.color = backgroundColor;

	vec3 LDcorn = vec3(obj.arr1.x, obj.arr1.y, obj.arr1.z), 
		 RDcorn = vec3(obj.arr1.w, obj.arr2.x, obj.arr2.y), 
		 LUcorn = vec3(obj.arr2.z, obj.arr2.w, obj.arr3.x), 
		 normal = vec3(obj.arr3.y, obj.arr3.z, obj.arr3.w);

	return NormalRectCheckCollision(Ray, LDcorn, RDcorn, LUcorn, normal, objIndex);
}

returnCollision ray_cast_box(Object obj, ray Ray, int objIndex)
{
	returnCollision rtrn;
	rtrn.collRes = 0;
	rtrn.color = backgroundColor;

	vec3 dot1 = vec3(obj.arr1.x, obj.arr1.y, obj.arr1.z),
		 dot2 = vec3(obj.arr1.w, obj.arr2.x, obj.arr2.y),
		 dot3 = vec3(obj.arr2.z, obj.arr2.w, obj.arr3.x),
		 dot4 = vec3(obj.arr3.y, obj.arr3.z, obj.arr3.w),
		 dot5 = dot4 + (dot2 - dot1),
		 dot6 = dot4 + (dot3 - dot1),
		 dot7 = dot6 + (dot2 - dot1), vecColl;

	vec3 n1 = normalVec3(cross(dot2 - dot1, dot4 - dot1)), 
		 n2 = normalVec3(cross(dot3 - dot1, dot4 - dot1)), 
		 n3 = normalVec3(cross(dot3 - dot1, dot2 - dot1)), 
		 n4 = normalVec3(cross(dot6 - dot4, dot5 - dot4)), 
		 n5 = normalVec3(cross(dot2 - dot5, dot7 - dot5)), 
		 n6 = normalVec3(cross(dot3 - dot6, dot7 - dot6));

	returnCollision p1 = NormalRectCheckCollision(Ray, dot1, dot2, dot4, n1, objIndex),
					p2 = NormalRectCheckCollision(Ray, dot1, dot3, dot4, n2, objIndex),
					p3 = NormalRectCheckCollision(Ray, dot1, dot3, dot2, n3, objIndex),
					p4 = NormalRectCheckCollision(Ray, dot4, dot6, dot5, n4, objIndex),
					p5 = NormalRectCheckCollision(Ray, dot5, dot2, dot7, n5, objIndex),
					p6 = NormalRectCheckCollision(Ray, dot6, dot3, dot7, n6, objIndex);

	double minDist = RAY_INF;
	returnCollision nearestColDot;

	if (p1.collRes == 1 && length(p1.pos - Ray.org) < minDist) {
		minDist = length(p1.pos - Ray.org);
		nearestColDot = p1;
	}
	if (p2.collRes == 1 && length(p2.pos - Ray.org) < minDist) {
		minDist = length(p2.pos - Ray.org);
		nearestColDot = p2;
	}
	if (p3.collRes == 1 && length(p3.pos - Ray.org) < minDist) {
		minDist = length(p3.pos - Ray.org);
		nearestColDot = p3;
	}
	if (p4.collRes == 1 && length(p4.pos - Ray.org) < minDist) {
		minDist = length(p4.pos - Ray.org);
		nearestColDot = p4;
	}
	if (p5.collRes == 1 && length(p5.pos - Ray.org) < minDist) {
		minDist = length(p5.pos - Ray.org);
		nearestColDot = p5;
	}
	if (p6.collRes == 1 && length(p6.pos - Ray.org) < minDist) {
		minDist = length(p6.pos - Ray.org);
		nearestColDot = p6;
	}

	if (minDist == RAY_INF)
		return rtrn;
	else {
		nearestColDot.color = vec3(1, 1, 0);
		return nearestColDot;
	}
}

returnCollision ray_cast_plane(Object obj, ray Ray, int objIndex)
{
	returnCollision rtrn;
	rtrn.collRes = 0;
	rtrn.color = backgroundColor;

	vec3 planeNormal = vec3(obj.arr1.x, obj.arr1.y, obj.arr1.z);
	vec3 planeCenter = vec3(obj.arr1.w, obj.arr2.x, obj.arr3.y);
	if (dot(Ray.dir, planeNormal) == 0)
		return rtrn;

	float d = (-1) * (dot(planeNormal, planeCenter)),
		t = -(dot(Ray.org, planeNormal) + d) / (dot(Ray.dir, planeNormal));

	if (t <= 0 || t > RAY_INF)
		return rtrn;

	rtrn.pos = Ray.org + Ray.dir * (t - RAY_SHIFT);
	rtrn.collRes = 1;
	//rtrn.color = Materials[objIndex].arr1.xyz;
	rtrn.color = vec3(0, 0, 1);
	return rtrn;
}

returnCollision ray_cast_object( Object obj, ray Ray, int objIndex)
{
	if (obj.type == SPHERE)
	{
		return ray_cast_sphere(obj, Ray, objIndex);
	}
	else if (obj.type == RECTANGLE)
	{
		return ray_cast_rectangle(obj, Ray, objIndex);
	}
	else if (obj.type == PLANE)
	{
		return ray_cast_plane(obj, Ray, objIndex);
	}
	else if (obj.type == BOX)
	{
		return ray_cast_box(obj, Ray, objIndex);
	}
	else
	{
		returnCollision rtrn;
		rtrn.collRes = 0;
		rtrn.color = backgroundColor;

		return rtrn;
	}
}

vec3 RayCast(ray Ray, int depth) 
{
	float nearestDist = RAY_INF;
	vec3 returnedColor = backgroundColor;

	for (int i = 0; i < Objects_size; i++)
	{
		returnCollision rtrn = ray_cast_object(Objects[i], Ray, i);
		if (rtrn.collRes != 0 && length(rtrn.pos - Ray.org) < nearestDist) 
		{
			nearestDist = length(rtrn.pos - Ray.org);
			returnedColor = rtrn.color;
		}
	}

	return returnedColor;
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
		zCoord = 0 + scrCoordH / 2 - pixelSize * ((WindowSize.y - gl_FragCoord.y) + 1.0 / 2);
	
	vec3 screenPointCrd = vec3(xCoord, yCoord, zCoord);
    screen_ray.org = screenPointCrd;
	screen_ray.dir = normalize(screenPointCrd - cam);
    for (int i = 0; i < Objects_size; i++)
	{
		FragColor = vec4(RayCast(screen_ray, 0), 1);
	}
}