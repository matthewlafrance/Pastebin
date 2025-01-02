use reqwest;
use tokio;
use clap::Parser;
use std::io::{self, Read};

#[tokio::main]
async fn main() -> reqwest::Result<()> {
    run().await
}

async fn run() -> reqwest::Result<()> {
    let cli = pastebin_client::CLI::parse();
    let mut url = "http://127.0.0.1:8000/".to_owned();
    let client = reqwest::Client::new();

    match cli.command {
        pastebin_client::Commands::view { id: i } => {
            url.push_str("view/");
            url.push_str(&i);
            let response = reqwest::get(url).await?.text().await?;
            println!("{}", response);
        },
        pastebin_client::Commands::paste { id: i } => {
            url.push_str("paste/");
            url.push_str(&i);
            let mut text = String::new();
            io::stdin().read_to_string(&mut text);
            let response = client.post(url).body(text).send().await?;
        },
        pastebin_client::Commands::delete { id: i } => {
            url.push_str("delete/");
            url.push_str(&i);
            let response = client.delete(url).send().await?;
        },
        pastebin_client::Commands::list => {
            url.push_str("list");
            let response = reqwest::get(url).await?.text().await?;
            println!("{}", response);
        },
    };
    Ok(())
}
