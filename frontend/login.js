// Login functionality for Library Management System

// Check if already logged in
document.addEventListener('DOMContentLoaded', () => {
    const token = localStorage.getItem('authToken');
    if (token) {
        // Verify token is still valid
        verifyTokenAndRedirect(token);
    }
});

// Form submission handler
document.getElementById('loginForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    const username = document.getElementById('username').value.trim();
    const password = document.getElementById('password').value;
    const adminKey = document.getElementById('adminKey').value.trim();
    
    if (!username || !password) {
        showError('Please enter both username/email and password');
        return;
    }
    
    await performLogin(username, password, adminKey);
});

// Register link handler
document.getElementById('showRegisterLink').addEventListener('click', (e) => {
    e.preventDefault();
    window.location.href = 'register.html';
});

async function performLogin(username, password, adminKey) {
    const loginBtn = document.getElementById('loginBtn');
    const errorMessage = document.getElementById('errorMessage');
    const successMessage = document.getElementById('successMessage');
    const loadingSpinner = document.getElementById('loadingSpinner');
    
    // Hide messages
    errorMessage.style.display = 'none';
    successMessage.style.display = 'none';
    
    // Show loading
    loadingSpinner.style.display = 'block';
    loginBtn.disabled = true;
    loginBtn.textContent = 'Signing In...';
    
    try {
        const payload = {
            username_or_email: username,
            password: password
        };
        
        // Include admin key only if provided
        if (adminKey) {
            payload.admin_key = adminKey;
        }
        
        console.log('Attempting login with payload:', { ...payload, password: '***' });
        
        const response = await fetch(`${window.ENV.API_BASE}/auth/login`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(payload)
        });
        
        const data = await response.json();
        console.log('Login response:', data);
        
        if (!response.ok) {
            throw new Error(data.error || data.message || 'Login failed');
        }
        
        // Check if response has token
        if (!data.token) {
            console.error('No token in response:', data);
            throw new Error('No authentication token received');
        }
        
        console.log('Login successful! Token:', data.token.substring(0, 20) + '...');
        console.log('User role:', data.role);
        
        // Store authentication data
        localStorage.setItem('authToken', data.token);
        localStorage.setItem('userRole', data.role);
        localStorage.setItem('userID', data.user.userID);
        localStorage.setItem('username', data.user.username);
        localStorage.setItem('userEmail', data.user.email);
        localStorage.setItem('userName', data.user.name);
        
        console.log('Stored in localStorage:', {
            role: data.role,
            userID: data.user.userID,
            username: data.user.username
        });
        
        // Show success message
        successMessage.textContent = `Welcome ${data.user.name}! Redirecting to your dashboard...`;
        successMessage.style.display = 'block';
        
        // Redirect based on role
        setTimeout(() => {
            const redirectUrl = data.role === 'ADMIN' ? 'index.html' : 'user-dashboard.html';
            console.log('Redirecting to:', redirectUrl);
            window.location.href = redirectUrl;
        }, 1000);
        
    } catch (error) {
        console.error('Login error:', error);
        showError(error.message || 'Login failed. Please try again.');
        
        // Reset button
        loginBtn.disabled = false;
        loginBtn.textContent = 'Sign In';
    } finally {
        loadingSpinner.style.display = 'none';
    }
}

async function verifyTokenAndRedirect(token) {
    try {
        const response = await fetch(`${window.ENV.API_BASE}/auth/me`, {
            method: 'GET',
            headers: {
                'Authorization': `Bearer ${token}`
            }
        });
        
        if (response.ok) {
            const data = await response.json();
            
            // Update stored data
            if (data.user) {
                localStorage.setItem('userRole', data.user.role);
                localStorage.setItem('userID', data.user.userID);
                localStorage.setItem('username', data.user.username);
                localStorage.setItem('userEmail', data.user.email);
                localStorage.setItem('userName', data.user.name);
                
                // Redirect based on role
                if (data.user.role === 'ADMIN') {
                    window.location.href = 'index.html';
                } else {
                    window.location.href = 'user-dashboard.html';
                }
            }
        } else {
            // Token is invalid, clear it
            clearAuthData();
        }
    } catch (error) {
        console.error('Token verification error:', error);
        clearAuthData();
    }
}

function showError(message) {
    const errorMessage = document.getElementById('errorMessage');
    errorMessage.textContent = message;
    errorMessage.style.display = 'block';
    
    // Auto-hide after 5 seconds
    setTimeout(() => {
        errorMessage.style.display = 'none';
    }, 5000);
}

function clearAuthData() {
    localStorage.removeItem('authToken');
    localStorage.removeItem('userRole');
    localStorage.removeItem('userID');
    localStorage.removeItem('username');
    localStorage.removeItem('userEmail');
    localStorage.removeItem('userName');
}
