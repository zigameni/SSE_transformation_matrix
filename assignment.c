
#include </Users/ziga/AppData/Local/Programs/Python/Python311/include/Python.h>
// #include <xmmintrin.h>
#include <immintrin.h>
#include <stdio.h>

void print_m128(__m128 a);
float get_first_elem_m128(__m128 a);
float sum_m128(__m128 a);
void transform_vertices_simd(float* vertex_buffer, Py_ssize_t num_elements, float* transform_matrix) {

    Py_ssize_t num_vertices = num_elements /3;
    printf("Number of elements: %zd \n", num_elements);
    printf("Number of vertices: %zd \n", num_vertices);

    // Load the transformation matrix into an SSE register
    __m128 mat[4];
    mat[0] = _mm_load_ps(transform_matrix);         // 2.000000 0.000000 0.000000 0.000000
    mat[1] = _mm_load_ps(transform_matrix + 4);     // 0.000000 2.000000 0.000000 0.000000
    mat[2] = _mm_load_ps(transform_matrix + 8);     // 0.000000 0.000000 2.000000 0.000000
    mat[3] = _mm_load_ps(transform_matrix + 12);    // 0.000000 0.000000 0.000000 1.000000


    //print_m128(_mm_dp_ps(x, mat[0], 0b11110001));
    printf("First 12 elements: \n");
    printf("%f, %f, %f, %f,%f, %f, %f, %f,%f, %f, %f, %f\n", 
    vertex_buffer[0], vertex_buffer[1], vertex_buffer[2], vertex_buffer[3], vertex_buffer[4], vertex_buffer[5], vertex_buffer[6], vertex_buffer[7],vertex_buffer[8], vertex_buffer[9], vertex_buffer[10], vertex_buffer[11]);
    
    // printf("\nLast 12 elements: \n");
    // printf("%f, %f, %f, %f,%f, %f, %f, %f,%f, %f, %f, %f\n", 
    // vertex_buffer[num_elements-12], vertex_buffer[num_elements-11], vertex_buffer[num_elements-10], 
    // vertex_buffer[num_elements-9], vertex_buffer[num_elements-8], vertex_buffer[num_elements-7], 
    // vertex_buffer[num_elements-6], vertex_buffer[num_elements-5],vertex_buffer[num_elements-4], 
    // vertex_buffer[num_elements-3], vertex_buffer[num_elements-2], vertex_buffer[num_elements-1]);
    
    printf("\n No seg 1\n");

    int first = 1;
    Py_ssize_t index = 0;


    for(Py_ssize_t i=0; i<num_vertices; i++){
        // Load four vertices into an SSE vector
        __m128 v_row = _mm_loadu_ps(vertex_buffer + i * 3);
        
        if(i<5){
            printf("elems %zd: ",i );
            print_m128(v_row);
        }

        // Convert vertices to homogeneous coordinates
        __m128 v_homog = _mm_set_ps(1.0f, vertex_buffer[i*3 + 2], vertex_buffer[i*3 + 1], vertex_buffer[i*3]);
        if(i<5){
            printf("First 12 elements: \n");
    printf("%f, %f, %f, %f,%f, %f, %f, %f,%f, %f, %f, %f\n", 
    vertex_buffer[0], vertex_buffer[1], vertex_buffer[2], vertex_buffer[3], vertex_buffer[4], vertex_buffer[5], vertex_buffer[6], vertex_buffer[7],vertex_buffer[8], vertex_buffer[9], vertex_buffer[10], vertex_buffer[11]);
    
            printf("homog %zd: ",i );
            print_m128(v_homog);
        }
        // Multiply vertices by transformation matrix
        __m128 v_row0 = _mm_mul_ps(mat[0], v_homog);
        __m128 v_row1 = _mm_mul_ps(mat[1], v_homog);
        __m128 v_row2 = _mm_mul_ps(mat[2], v_homog);
        __m128 v_row3 = _mm_mul_ps(mat[3], v_homog);
        
        float v_sum0 = sum_m128(v_row0);
        float v_sum1 = sum_m128(v_row1);
        float v_sum2 = sum_m128(v_row2);
        float v_sum3 = sum_m128(v_row3);
        if(i<5){
            printf("\nv_sum0: %f", v_sum0);
            printf("\nv_sum1: %f", v_sum1);
            printf("\nv_sum2: %f", v_sum2);
            printf("\nv_sum3: %f", v_sum3);

        }
        __m128 res = _mm_setr_ps(v_sum0, v_sum1, v_sum2, vertex_buffer[i*3+3]);
        
        if(i<5){
        printf("\nRows: \n");
        print_m128(v_row0);
        print_m128(v_row1);
        print_m128(v_row2);
        print_m128(v_row3);
        }


        __m128 v_result = _mm_add_ps(_mm_add_ps(v_row0, v_row1), _mm_add_ps(v_row2, v_row3));
        float f[4];
        _mm_store_ps(f, v_result);
        //__m128 res = _mm_setr_ps(f[0], f[1], f[2], vertex_buffer[i*3+3]);


        if(i<5){
            printf("result %zd: ",i );
            print_m128(v_result);
            printf("res %zd: ",i );
            print_m128(res);
        }

        // Store transformed vertices back into the original array
        _mm_storeu_ps(vertex_buffer + i * 3, res);
        if(i<5){
            printf("\nAfter storing: First 12 elements: \n");
    printf("%f, %f, %f, %f,%f, %f, %f, %f,%f, %f, %f, %f\n", 
    vertex_buffer[0], vertex_buffer[1], vertex_buffer[2], vertex_buffer[3], vertex_buffer[4], vertex_buffer[5], vertex_buffer[6], vertex_buffer[7],vertex_buffer[8], vertex_buffer[9], vertex_buffer[10], vertex_buffer[11]);
        }
    }
    
    printf("\n First 12 elements again: \n");
    printf("%f, %f, %f, %f,%f, %f, %f, %f,%f, %f, %f, %f\n", 
    vertex_buffer[0], vertex_buffer[1], vertex_buffer[2], vertex_buffer[3], vertex_buffer[4], vertex_buffer[5], vertex_buffer[6], vertex_buffer[7],vertex_buffer[8], vertex_buffer[9], vertex_buffer[10], vertex_buffer[11]);
    

    // printf("\nLast 12 elements: \n");
    // printf("%f, %f, %f, %f,%f, %f, %f, %f,%f, %f, %f, %f\n", 
    // vertex_buffer[num_vertices-12], vertex_buffer[num_vertices-11], vertex_buffer[num_vertices-10], 
    // vertex_buffer[num_vertices-9], vertex_buffer[num_vertices-8], vertex_buffer[num_vertices-7], 
    // vertex_buffer[num_vertices-6], vertex_buffer[num_vertices-5],vertex_buffer[num_vertices-4], 
    // vertex_buffer[num_vertices-3], vertex_buffer[num_vertices-2], vertex_buffer[num_vertices-1]);
    
    

}


union U {
        __m128 v;
        float a[4];
};

void print_m128(__m128 a){
    //float data[] = {1.0f, 2.0f, 3.0f, 4.0f};
    //__m128 a = _mm_loadu_ps(&data[0]);

    float f[4];
    _mm_store_ps(f, a);
    printf("%f %f %f %f \n",f[0],f[1],f[2],f[3]);
}
int j = 0; 
float sum_m128(__m128 a){
    float f[4]={0.0, 0.0, 0.0, 0.0};
    // print_m128(a);
    _mm_store_ps(f, a);
    float sum = f[0]+f[1]+f[2]+f[3];
    if(j<10){
        printf("\nElements of float: %f, %f, %f, %f", f[0], f[1], f[2], f[3]);
        printf("the sum is: %f", sum);
        j++;  
    }
    
    
    return sum;
}

float get_first_elem_m128(__m128 a){
    float f[4];
    _mm_store_ps(f, a);
    return f[0];
}
void print_buffer_shape(Py_buffer buffer){
    printf("Shape: (%zd, )", buffer.shape[0]);
}

static PyObject *transform_vertices(PyObject *self, PyObject *args)
{
    PyObject *vtxarray_obj;
    PyObject *mat4_obj;
    Py_buffer vtxarray_buf;
    Py_buffer mat4_buf;
    // Try to read two arguments: the vertex array and the transformation matrix
    if (!PyArg_ParseTuple(args, "OO", &vtxarray_obj, &mat4_obj))
        return NULL;
    // Try to read the objects as contiguous buffers
    if (PyObject_GetBuffer(vtxarray_obj, &vtxarray_buf,
                        PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1 ||
        PyObject_GetBuffer(mat4_obj, &mat4_buf,
                        PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1)
        return NULL;
    // Check that the vertex buffer has one dimension and contains floats ("f")
    if (vtxarray_buf.ndim != 1 || strcmp(vtxarray_buf.format, "f") != 0)
    {
        PyErr_SetString(PyExc_TypeError,
                        "Vertex buffer must be a flat array of floats");
        PyBuffer_Release(&vtxarray_buf);
        return NULL;
    }
    // Check that the transformation matrix buffer has 16 float elements
    if (mat4_buf.ndim != 1 || mat4_buf.shape[0] != 16 ||
        strcmp(mat4_buf.format, "f") != 0)
    {
        PyErr_SetString(PyExc_TypeError,
                        "Transformation matrix must be an array of 16 floats");
        PyBuffer_Release(&mat4_buf);
        return NULL;
    }
    // Call the actual processing function
    //transform_vertices_simd(vtxarray_buf.buf, vtxarray_buf.shape[0], mat4_buf.buf);
    printf("Here: \n");
    //printf();
    //print_buffer_shape(vtxarray_buf);
    transform_vertices_simd((float*)vtxarray_buf.buf, vtxarray_buf.shape[0], (float*)mat4_buf.buf);
    //transform_vertices_simd((float*)vtxarray_buf.buf, vtxarray_buf.len, (float*)mat4_buf.buf);
    
    //test1();
    //test2();

    // transform_vertices_simd();
    //  Clean up resources and return to Python code
    PyBuffer_Release(&vtxarray_buf);
    PyBuffer_Release(&mat4_buf);
    Py_RETURN_NONE;
}

PyMODINIT_FUNC PyInit_assignment(void)
{
    static PyMethodDef module_methods[] = {{"transform_vertices",
                                            (PyCFunction)transform_vertices,
                                            METH_VARARGS,
                                            "Transform vertex array"},
                                           {NULL}};
    static struct PyModuleDef module_def = {
        PyModuleDef_HEAD_INIT,
        "assignment",
        "This module contains my assignment code",
        -1,
        module_methods};
    return PyModule_Create(&module_def);
}
