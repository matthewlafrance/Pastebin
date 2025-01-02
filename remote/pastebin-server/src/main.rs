use std::io::{BufWriter, BufReader, Write, Read};
use std::fs::{self, File};
use std::env;
use rocket;

#[rocket::get("/view/<ID>")]
fn view(ID: &str) -> Result<String, String> {
    let mut text = String::new();
    let file = File::open(get_full_path(ID)).map_err(|e| e.to_string())?;
    let mut reader = BufReader::new(file);
    reader.read_to_string(&mut text).map_err(|e| e.to_string())?;
    Ok(text)
}

#[rocket::post("/paste/<ID>", data="<text>")]
fn paste(ID: &str, text: &str) -> Result<(), String> {
    let path = get_path();
    let full_path = get_full_path(ID);
    
    let new_paste = match fs::exists(path.clone()).map_err(|e| e.to_string())? {
        true => {
            File::create_new(full_path).map_err(|e| e.to_string())?
        },
        false => {
            fs::create_dir(path).map_err(|e| e.to_string())?;
            File::create_new(full_path).map_err(|e| e.to_string())?
        },
    };

    let mut stream = BufWriter::new(new_paste);
    stream.write(text.as_bytes()).map_err(|e| e.to_string())?;

    Ok(())
}

#[rocket::delete("/delete/<ID>")]
fn delete(ID: &str) -> Result<(), String> {
    fs::remove_file(get_full_path(ID)).map_err(|e| e.to_string())
}

#[rocket::get("/list")]
fn list() -> Result<String, String> {
    let mut list = String::new();
    for entry in fs::read_dir(get_path()).map_err(|e| e.to_string())? {
        list.push_str(&entry.map_err(|e| e.to_string())?.file_name().into_string().unwrap());
        list.push_str("\n");
    }
    Ok(list)
}

fn get_path() -> String {
    let mut path = String::from("/");
    let mut env = env::var("HOME").unwrap();
    env.push_str("/.pastebin");
    path.push_str(&env);
    path
}

fn get_full_path(ID: &str) -> String {
    let mut path = get_path();
    path.push_str("/");
    path.push_str(ID);
    path
}

#[rocket::launch]
fn launch() -> _ {
    rocket::build().mount("/", rocket::routes![view, paste, delete, list])
}
