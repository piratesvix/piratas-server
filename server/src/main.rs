use std::io::{ErrorKind, Read, Write};
use std::net::TcpListener;
use std::sync::mpsc;
use std::thread;

// criando uma constante de nosso host local
const host: &str = "127.0.0.1:4200";
// tamanho permitido de 32 bits por mensagem
const msg_size: usize = 32;

fn main() {
    // atribuir conexão tcp com host especifícado
    let server = TcpListener::bind(host).expect("falha ao tentar vincular com host");
    server.set_nonblocking(true).expect("falha ao tentar iniciar o non-blocking")

    // elaborando forma de permitir mais de dois clientes se conectar ao servidor
    let mut piratas = vec![];
    // instanciar canal para receber torpedos de strings
    let (tx, rx) = mpsc::channel::<String>;

    loop {
        // políticas para permissão na conexão entre clientes e o servidor
        // por socket de fluxo TCP conectado ao endereço do cliente
        if let Ok((mut socket, addr)) = server.accept() {
            println!("Pirata {} conectado", addr)
        }
    }

}
