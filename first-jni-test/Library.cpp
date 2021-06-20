#include <jni.h>
#include <iostream>
#include "Library.h"

JNIEXPORT jint JNICALL Java_Library_squared(JNIEnv *env, jobject thisObj, jint input)
{
	return input * input;
}

JNIEXPORT jlong JNICALL Java_Library_fibonacci(JNIEnv *env, jobject this_, jint num)
{
	jlong before = 0;
	jlong before_before = 1;
	jlong fib = 0;
	for (jint i = 0; i < num; i++)
	{
		fib = before + before_before;
		before_before = before;
		before = fib;
	}
	return fib;
}