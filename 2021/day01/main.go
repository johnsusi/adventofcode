package main

import (
	"bufio"
	"io"
	"os"
	"strconv"
)

func parse(infile io.Reader) []int {
	scanner := bufio.NewScanner(infile)
	scanner.Split(bufio.ScanWords)
	var result []int
	for scanner.Scan() {
		x, err := strconv.Atoi(scanner.Text())
		if err != nil {
			panic(err)
		}
		result = append(result, x)
	}
	if scanner.Err() != nil {
		panic(scanner.Err())
	}
	return result
}

func part1(input []int) int {
	result := 0
	for i := 1; i < len(input); i++ {
		if input[i-1] < input[i] {
			result++
		}
	}
	return result
}

func part2(input []int) int {
	var windows []int
	for i := 0; i < len(input)-2; i++ {
		window := 0
		for j := 0; j < 3; j++ {
			window += input[i+j]
		}
		windows = append(windows, window)
	}
	return part1(windows)
}

func main() {
	infile, _ := os.Open("2021/day01/data.txt")
	input := parse(infile)
	println(part1(input)) // 1400
	println(part2(input)) // 1429
}
