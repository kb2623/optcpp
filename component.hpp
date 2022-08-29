#ifndef _COMPONENT_H_
#define _COMPONENT_H_

template <typename T>
concept Component = requires(T a, T b) {
	a + b -> T;
	a - b -> T;
	a * b -> T;
	a / b -> T;
	a % b -> T;
};

#endif
