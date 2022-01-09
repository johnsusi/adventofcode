use std::fs::File;
use std::io::BufRead;
use std::io::BufReader;

fn parse(path: &str) -> Vec<i64> {
    let file = File::open(path).expect("file wasn't found.");
    return BufReader::new(file)
        .lines()
        .map(|line| line.unwrap().parse::<i64>().unwrap())
        .collect();
}

fn part1(input: &Vec<i64>) -> i64 {
    return input
        .iter()
        .zip(input.iter().skip(1))
        .filter(|(a, b)| a < b)
        .count() as i64;
}

fn part2(input: &Vec<i64>) -> i64 {
    let windows = input.windows(3).map(|w| w.iter().sum()).collect();
    return part1(&windows);
}

fn main() {
    let input = parse("2021/day01/data.txt");
    println!("{}", part1(&input)); // 1400
    println!("{}", part2(&input)); // 1429
}
