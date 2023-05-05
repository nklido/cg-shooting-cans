#ifndef _ISECT_
#define _ISECT_

#include "glm/glm.hpp"

/*! Finds an intersection point (if any) between a ray and a triangle.
   
    A ray is defined by its origin and a (normalized) direction. 

	\param origin is the starting point of the ray.
	\param direction is a normalized vector pointing towards the direction the ray travels
	\param p0 is the first vertex of the triangle.
	\param p1 is the second vertex of the triangle.
	\param p2 is the third vertex of the triangle.
	\param n is the reported normal at the hit location (if any).
	\param t is the distance at which the intersection was found (if any).
	\param beta is the barycentric coordinate of the second vertex.
	\param gamma is the barycentric coordinate of the third vertex.

	Note that the barycentric coordinate of the first vertex is 1 - beta - gamma.

	\return true if an intersection between the ray and the triangle was found, false otherwise. Intersections  
	behind the origin are no reported and the function returns false.
 */
bool intersect_triangle(const glm::vec3& origin,const glm::vec3& direction,const glm::vec3& p0,const glm::vec3& p1,const glm::vec3& p2,glm::vec3& n,
	float&  t,
	float&  beta,
	float&  gamma);

/*! Calculates the closest intersection and the normal vector at it (if any) between a ray and a triangle "soup" that represents a surface. 

   \param origin is the starting point of the ray.
   \param direction is a normalized vector pointing towards the direction the ray travels
   \param geometry is an array of glm::vec3 vertices that represent the surface. For num_triangles, 3 X num_triangles vertices must be provided, 
   so common vertices between triangles must be replicated.
   \param num_triangles is the number of triangles that the geometry array represents.
   \param hit is the updated hit location, in case of a successful intersection.
   \param hit_normal is the normal at the hot location, in case of a successful intersection.

   \return true if an intersection was found, otherwise. Intersections  
   behind the origin are no reported and the function returns false.
*/
bool intersect_triangles(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3 * geometry, unsigned int num_triangles,
	glm::vec3 & hit,
	glm::vec3 & hit_normal);

/*! Detects an intersection closer than a user-defined distance between a ray and a triangle "soup" that represents a surface.

	The function exits early upon the first intersection detected, so it is faster than intersect_triangles(). However it cannot report the 
	location at which the intersection occured, since the detected hit may not be the closest one to the ray origin.

    \param origin is the starting point of the ray.
	\param direction is a normalized vector pointing towards the direction the ray travels
	\param geometry is an array of glm::vec3 vertices that represent the surface. For num_triangles, 3 X num_triangles vertices must be provided,
	so common vertices between triangles must be replicated.
	\param num_triangles is the number of triangles that the geometry array represents.
	\param distance defines the offset from the ray origin below which a potential intersection is reported. Collisions fartherthan this distance are ignored.

	\return true if an intersection was found, otherwise. Intersections
	behind the origin are no reported and the function returns false.
*/
bool intersect_triangles_collision_only(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3 * geometry,
	unsigned int num_triangles,
	float distance);
#endif