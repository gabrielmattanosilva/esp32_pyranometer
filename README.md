# Criar Conta e Configurar ThingSpeak

https://thingspeak.mathworks.com/

# Compilação do Projeto no PlatformIO

Para compilar o projeto utilizando o PlatformIO, é necessário criar um arquivo `credentials.h` no diretório "include". Esse arquivo deve contero número do canal, a chave da API de escrita do e as credenciais da sua rede Wi-Fi.

## Passos para Configuração

1. No diretório "include" do projeto, crie um arquivo chamado `credentials.h`.
2. Dentro desse arquivo, substitua `nome_da_rede` e `senha` pelas informações da sua rede Wi-Fi, `12334567` pelo número do canal e `ABCDEFG123456789` pela chave da API de escrita do ThingSpeak:

   ```bash
   #ifndef CREDENTIALS_H
   #define CREDENTIALS_H

   #define WIFI_SSID "nome_da_rede"
   #define WIFI_PASSWORD "senha"
   #define CHANNEL_NUMBER 12334567
   #define WRITE_API_KEY "ABCDEFG123456789"

   #endif // CREDENTIALS_H

3. Certifique-se de que o arquivo `credentials.h` está listado no seu .gitignore para que suas credenciais não sejam enviadas para o repositório Git.

Após criar o arquivo com as credenciais, você pode compilar o projeto normalmente.
