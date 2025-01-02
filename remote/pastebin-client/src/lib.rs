use clap::{Parser, Subcommand};

#[derive(Parser)]
pub struct CLI {
    #[arg(short, long)]
    pub test: bool,
    #[command(subcommand)]
    pub command: Commands,
}

#[derive(Subcommand)]
pub enum Commands {
    view {
        id: String,
    },
    paste {
        id: String,
    },
    delete{
        id: String,
    },
    list,
}
