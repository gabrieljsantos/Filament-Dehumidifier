## pyinstaller pyinstaller --onefile local/update_html_ino.py
## creater executable
def update_html_in_esp32():
    html_file_path = 'index.html'
    esp32_ino_file_path = 'ESP32/ESP32.ino'
    
    # Abrir o arquivo HTML e ler seu conteúdo
    with open(html_file_path, 'r') as html_file:
        html_content = html_file.read()
    
    # Abrir o arquivo ESP32.ino e ler seu conteúdo
    with open(esp32_ino_file_path, 'r') as esp32_ino_file:
        esp32_ino_content = esp32_ino_file.read()
    
    # Encontrar o trecho do HTML_PAGE no ESP32.ino
    start_tag = 'const char HTML_PAGE[] = R"rawliteral('
    end_tag = ')rawliteral";'
    start_index = esp32_ino_content.find(start_tag)
    end_index = esp32_ino_content.find(end_tag, start_index + len(start_tag))
    
    if start_index == -1 or end_index == -1:
        print('Erro: Não foi possível encontrar o trecho HTML_PAGE no arquivo ESP32.ino.')
        return
    
    # Construir o novo conteúdo com o HTML atualizado
    new_esp32_ino_content = (esp32_ino_content[:start_index + len(start_tag)]
                             + '\n' + html_content + '\n'
                             + esp32_ino_content[end_index:])
    
    # Escrever o conteúdo atualizado de volta no arquivo ESP32.ino
    with open(esp32_ino_file_path, 'w') as esp32_ino_file:
        esp32_ino_file.write(new_esp32_ino_content)
    
    print('Conteúdo do arquivo ESP32.ino atualizado com sucesso com o conteúdo de index.html.')

if __name__ == '__main__':
    update_html_in_esp32()
