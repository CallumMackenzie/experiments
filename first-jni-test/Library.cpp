#include <jni.h>
#include <iostream>
#include "Library.h"

JNIEXPORT jlongArray JNICALL Java_Library_first_1fib_1cpp(JNIEnv *env, jobject jthis, jint len)
{
	jlongArray jlarray = env->NewLongArray(len);
	jlong *arr = new jlong[len];

	for (jint i = 0; i < len; i++)
	{
		if (i == 0)
			arr[i] = 0;
		else if (i == 1)
			arr[i] = 1;
		else
			arr[i] = arr[i - 1] + arr[i - 2];
	}
	env->SetLongArrayRegion(jlarray, 0, len, arr);

	return jlarray;
}