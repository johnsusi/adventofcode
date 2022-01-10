extern crate regex;

use regex::Regex;
use std::fs;

fn parse(path: &str) -> Vec<(String, i64)> {
    let contents = fs::read_to_string(path).expect("Something went wrong reading the file");
    let re = Regex::new(r"(\w+) (\d+)").unwrap();
    return re
        .captures_iter(&contents)
        .map(|cap| (cap[1].to_string(), cap[2].parse::<i64>().unwrap()))
        .collect();
}

fn part1(input: &Vec<(String, i64)>) -> i64 {
    let (depth, horizontal) = input.iter().fold((0i64, 0i64), |a, v| {
        let (depth, horizontal) = a;
        let (command, value) = v;
        if *command == "up" {
            return (depth - value, horizontal);
        } else if *command == "down" {
            return (depth + value, horizontal);
        } else if *command == "forward" {
            return (depth, horizontal + value);
        }
        panic!();
    });
    return horizontal * depth;
}

fn part2(input: &Vec<(String, i64)>) -> i64 {
    let (depth, horizontal, _) = input.iter().fold((0i64, 0i64, 0i64), |a, v| {
        let (depth, horizontal, aim) = a;
        let (command, value) = v;
        if *command == "up" {
            return (depth, horizontal, aim - value);
        } else if *command == "down" {
            return (depth, horizontal, aim + value);
        } else if *command == "forward" {
            return (depth + aim * value, horizontal + value, aim);
        }
        panic!();
    });
    return horizontal * depth;
}

fn main() {
    let input = parse("2021/day02/data.txt");
    println!("{}", part1(&input)); // 1840243
    println!("{}", part2(&input)); // 1727785422
}
