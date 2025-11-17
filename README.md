# Monitoramento de Redes com ESP32 e FreeRTOS

Projeto desenvolvido para monitorar a rede Wi-Fi conectada ao ESP32, verificando se a rede está cadastrada como segura. Caso o dispositivo conecte a uma rede não autorizada, um alerta é gerado via fila do FreeRTOS.

---

## 🧩 Descrição Geral

O sistema utiliza três tarefas FreeRTOS:

### 🔹 Monitoramento da Rede
Envia periodicamente o nome da rede atual através de uma fila.

### 🔹 Verificação de Segurança
Recebe a rede monitorada, compara com a lista de redes seguras e identifica possíveis ameaças.

### 🔹 Alerta de Rede Não Autorizada
Exibe mensagens de alerta quando uma rede não segura é detectada.

Todo o fluxo utiliza:
- Filas (queues)
- Semáforo de exclusão mútua (mutex)
- Execução em múltiplos núcleos (ESP32 dual-core)

---

## 🛠 Arquivos do Projeto

