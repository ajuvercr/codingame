use std::io;

macro_rules! parse_input {
    ($x:expr, $t:ty) => ($x.trim().parse::<$t>().unwrap())
}

#[derive(Clone, Debug)]
struct Hole {
    x: f64, s: f64, e: f64,
}

#[derive(Clone, Debug)]
struct Line {
    x: f64, f: f64,
}

fn max(x: f64, y: f64) -> f64 {
    if x > y {
        return x;
    }
    return y;
}

fn min(x: f64, y: f64) -> f64 {
    if x < y {
        return x;
    }
    return y;
}

fn apply(holes: &mut Vec<Hole>, hole: & Hole, light: &Line) {
    let start = hole.s * light.x / hole.x;
    let end = hole.e * light.x / hole.x;

    let mut at = 0.0;
    while at < end {
        let s = max(start, at);
        let e = min(end, at + light.f);

        if s < e {
            holes.push(
                Hole {
                x: light.x,
                s, e,
                }
            );
        }

        at += light.f * 2.0;
    }
}

fn apply_list(mut holes: Vec<Hole>, light: &Line) -> Vec<Hole> {
    let mut out = Vec::new();

    for hole in holes.iter_mut() {
        apply(&mut out, hole, light);
    }

    return out;
}

fn main() {
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();
    let speed = parse_input!(input_line, f64);
    eprintln!("speed {}", speed);
    let mut input_line = String::new();
    io::stdin().read_line(&mut input_line).unwrap();

    let light_count = parse_input!(input_line, usize);
    let mut lights = Vec::with_capacity(light_count);
    for _ in 0..light_count {

        let mut input_line = String::new();
        io::stdin().read_line(&mut input_line).unwrap();
        let inputs = input_line.split(" ").collect::<Vec<_>>();
        let x = parse_input!(inputs[0], f64);
        let f = parse_input!(inputs[1], f64);

        lights.push(Line { x: x / 1000.0, f: f / 3600.0 });
    }
    eprintln!("{}", lights.len());
    for l in lights.iter() {
        eprintln!("{} {}", l.x, l.f);
    }
    eprintln!("{:?}", lights);


    let mut tt = 0.0;
    let first_light = lights[0].clone();

    loop {
        let mut holes = Vec::new();
        holes.push(
            Hole {
                x: first_light.x,
                s: tt * (first_light.f * 2.0),
                e: tt * (first_light.f * 2.0) + first_light.f,
            }
        );
        tt += 1.0;

        for light in lights.iter().skip(1) {
            holes = apply_list(holes, light);
        }

        for hole in holes.iter() {
            let min_speed = hole.x / hole.e;
            let max_speed = hole.x / hole.s;

            eprintln!("{} {} {}", speed, min_speed, max_speed);
            if speed > min_speed.ceil() {
                let answer = min(speed, max_speed);
                println!("{}", answer.floor());

                return;
            }
        }
    }
}
