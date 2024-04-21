
use std::io::{self, Write};

pub fn repl_main() {
    
    println!("\nGEOMOD Kernel (C) Seb Hall 2024");
    println!("CLI v0.0.1 - DEVELOPMENT ONLY\n");

    let mut path: Vec<String> = Vec::new();
    
    loop {

        

        //print!(" {} > ", path);
        
        print!(" /");
        for i in 0..path.len() {
            print!("{}/", path[i]);
        }
        print!(" > ");

        let _ = io::stdout().flush();

        let mut input = String::new();
        
        match io::stdin().read_line(&mut input) {
            Ok(_goes_into_input_above) => {},
            Err(_no_updates_is_fine) => {},
        }

        let tokens: Vec<&str> = input.trim().split_whitespace().collect();

        if tokens.len() > 0 {
            match tokens[0].trim() {
                "exit" => {
                    println!("GEOMOD CLI v0.0.1");
                    return;
                },
                "cd" => {
                    if tokens.len() == 2 {
                        path.push(String::from(tokens[1].trim()));
                        //path.push(tokens[1].clone().trim());
                    }
                },
                "ls" => {
                    
                },
                "new" => {
                    command_new(tokens[1..tokens.len()].to_vec());
                },

                _ => {
                    println!("unknown command '{}'", tokens[0].trim());
                }
            }
        }

        
    }
}

pub fn command_new(tokens: Vec<&str>) {
    if tokens.len() == 0 {
        println!("\x1b[93mWARNING\x1b[0m: No arguments provided. Use new as follows: 'new [entity|vertex|edge|face] [name]'");
        return;
    }  

    if tokens.len() == 2 {
        match tokens[0].trim() {
            "entity" => {
                println!("Created ENTITY with name '{}'", tokens[1].trim());
            },
            "vertex" => {
                println!("Created VERTEX with name '{}'", tokens[1].trim());
            },
            "edge" => {
                println!("Created EDGE with name '{}'", tokens[1].trim());
            },
            "face" => {
                println!("Created FACE with name '{}'", tokens[1].trim());
           },
           _ => {
                println!("\x1b[93mWARNING\x1b[0m: Unknown object type '{}'. Use new as follows: 'new [entity|vertex|edge|face] [name]'", tokens[0].trim());
           }
        }
    } else if tokens.len() == 1 {
        println!("\x1b[93mWARNING\x1b[0m: Not enough arguments provided. Use new as follows: 'new [entity|vertex|edge|face] [name]'");
    } else {
        println!("\x1b[93mWARNING\x1b[0m: Too many arguments provided. Use new as follows: 'new [entity|vertex|edge|face] [name]'");
    }


}