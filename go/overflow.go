package main

import (
	"fmt"
)

func main () {
	var i int = 9223372036854775807;
	i = i + 1;
	fmt.Println(i);
	// ah so Go doesn't specially handle overflows
}
