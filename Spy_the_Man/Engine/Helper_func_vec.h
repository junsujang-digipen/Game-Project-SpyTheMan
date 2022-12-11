/*--------------------------------------------------------------
File Name: Helper_func_vec.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/2/2021
-----------------------------------------------------------------*/
#pragma once

namespace helper {
	template<typename vectorN>
	inline double dot(vectorN A, vectorN B) {
		double sum{};
		int length{A.length()};
		for (int i = 0; i < length;++i) {
			sum += A[i] * B[i];
		}
		return sum;
	}
	template<typename vectorN>
	inline double length(vectorN A) {
		return sqrt(dot(A,A));
	}
	template<typename vectorN>
	inline vectorN normalize(vectorN A) {
		return A / vectorN{ length(A) };
	}
}