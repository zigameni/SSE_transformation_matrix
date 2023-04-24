import trimesh, assignment
import numpy as np
from trimesh import Trimesh
import math



def as_mesh(scene_or_mesh):
    if isinstance(scene_or_mesh, trimesh.Scene):
        if len(scene_or_mesh.geometry) == 0:
            mesh = None  # empty scene
        else:
            # we lose texture information here
            mesh = trimesh.util.concatenate(
                tuple(trimesh.Trimesh(vertices=g.vertices, faces=g.faces)
                    for g in scene_or_mesh.geometry.values()))
    else:
        assert(isinstance(mesh, trimesh.Trimesh))
        mesh = scene_or_mesh
    return mesh

def apply_transformation_matrix(array, transformation_matrix):
    """
    Applies a transformation matrix to an array of floats.

    Parameters:
    array (numpy.ndarray): The array to apply the transformation to.
    transformation_matrix (numpy.ndarray): The transformation matrix to apply.

    Returns:
    numpy.ndarray: The transformed array.
    """
    # Ensure that the input array is a numpy array
    array = np.array(array)
    
    # Ensure that the transformation matrix is a numpy array
    transformation_matrix = np.array(transformation_matrix)
    
    # Apply the transformation matrix to the array
    transformed_array = np.dot(transformation_matrix, array)
    
    return transformed_array

model = trimesh.load("lamp.obj", force='mesh')
model.show()


# extract the vertices into a float buffer 
vertex_buffer = np.ravel(model.vertices.view(np.ndarray)).astype(np.float32);
print(f"Buffer size: {vertex_buffer.size}")
print (vertex_buffer);

# # The transformation matrix
# transformation_matrix = np.array([
# 2.0, 0.0, 0.0, 0.0,
# 0.0, 2.0, 0.0, 0.0,
# 0.0, 0.0, 2.0, 0.0,
# 0.0, 0.0, 0.0, 1.0
# ], dtype=np.float32)
angle = math.pi/2.0;

# Shear
transformation_matrix1 = np.array([
math.cos(angle), 0.0, math.sin(angle), 0.0,
0.0, 1.0, 0.0, 0.0,
-math.sin(angle), 0.0, math.cos(angle), 0.0,
0.0, 0.0, 0.0, 1.0
], dtype=np.float32)

transformation_matrix = np.array([
1.0, 0.0, 0.0, 0.0,
0.0, math.cos(angle), -math.sin(angle), 0.0,
0.0, math.sin(angle), math.cos(angle), 0.0,
0.0, 0.0, 0.0, 1.0
], dtype=np.float32)

transformation_matrix2 = np.array([
math.cos(angle), -math.sin(angle), 0.0, 0.0,
math.sin(angle), math.cos(angle), 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0
], dtype=np.float32)


# Transform the vertex buffer with your C processing function: 
assignment.transform_vertices(vertex_buffer, transformation_matrix2)
print("We got hertee")
# Reassign the transformed vertices to the model object and visualize it: 
print(vertex_buffer.size)
print("We got here");
model.vertices = vertex_buffer.reshape((-1, 3));
model.show();





















"""
#model = as_mesh(trimesh.load("lamp.obj"))
#model = trimesh.load("mug.obj")
# model.show()
vertex_buffer = np.ravel(model.vertices.view(np.ndarray)).astype(np.float32)
print(vertex_buffer);
transformation_matrix = np.array([
2.0, 0.0, 0.0, 0.0,
0.0, 2.0, 0.0, 0.0,
0.0, 0.0, 2.0, 0.0,
0.0, 0.0, 0.0, 1.0
], dtype=np.float32)



assignment.transform_vertices(vertex_buffer, transformation_matrix)

model.show()
#transformed_vertices = vertex_buffer.reshape(-1, 3);
#mesh = Trimesh(transformed_vertices);
#print(vertex_buffer);
# print(vertex_buffer);
print("\n")
print("We good!")


"""
 



