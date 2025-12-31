// Registration functionality for Library Management System

// Check if already logged in
document.addEventListener('DOMContentLoaded', () => {
    const token = localStorage.getItem('authToken');
    if (token) {
        // Redirect to appropriate dashboard
        const role = localStorage.getItem('userRole');
        if (role === 'ADMIN') {
            window.location.href = 'index.html';
        } else {
            window.location.href = 'user-dashboard.html';
        }
    }
});

// Form submission handler
document.getElementById('registerForm').addEventListener('submit', async (e) => {
    e.preventDefault();
    
    const username = document.getElementById('username').value.trim();
    const name = document.getElementById('name').value.trim();
    const email = document.getElementById('email').value.trim();
    const password = document.getElementById('password').value;
    const confirmPassword = document.getElementById('confirmPassword').value;
    const adminKey = document.getElementById('adminKey').value.trim();
    
    // Validation
    if (!username || !name || !email || !password) {
        showError('Please fill in all required fields');
        return;
    }
    
    if (password !== confirmPassword) {
        showError('Passwords do not match');
        return;
    }
    
    if (password.length < 6) {
        showError('Password must be at least 6 characters long');
        return;
    }
    
    if (!validateEmail(email)) {
        showError('Please enter a valid email address');
        return;
    }
    
    if (!validateUsername(username)) {
        showError('Username can only contain letters, numbers, and underscores');
        return;
    }
    
    await performRegistration(username, name, email, password, adminKey);
});

async function performRegistration(username, name, email, password, adminKey) {
    const registerBtn = document.getElementById('registerBtn');
    const errorMessage = document.getElementById('errorMessage');
    const successMessage = document.getElementById('successMessage');
    const loadingSpinner = document.getElementById('loadingSpinner');
    
    // Hide messages
    errorMessage.style.display = 'none';
    successMessage.style.display = 'none';
    
    // Show loading
    loadingSpinner.style.display = 'block';
    registerBtn.disabled = true;
    registerBtn.textContent = 'Creating Account...';
    
    try {
        const payload = {
            username: username,
            name: name,
            email: email,
            password: password,
            role: adminKey ? 'ADMIN' : 'USER'
        };
        
        // Include admin key only if provided
        if (adminKey) {
            payload.admin_key = adminKey;
        }
        
        const response = await fetch(`${window.ENV.API_BASE}/auth/register`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(payload)
        });
        
        const data = await response.json();
        
        if (!response.ok) {
            throw new Error(data.error || data.message || 'Registration failed');
        }
        
        // Show success message
        const role = data.user.role === 'ADMIN' ? 'Administrator' : 'User';
        successMessage.textContent = `Account created successfully as ${role}! Redirecting to login...`;
        successMessage.style.display = 'block';
        
        // Redirect to login page
        setTimeout(() => {
            window.location.href = 'login.html';
        }, 2000);
        
    } catch (error) {
        console.error('Registration error:', error);
        showError(error.message || 'Registration failed. Please try again.');
        
        // Reset button
        registerBtn.disabled = false;
        registerBtn.textContent = 'Create Account';
    } finally {
        loadingSpinner.style.display = 'none';
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

function validateEmail(email) {
    const re = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return re.test(email);
}

function validateUsername(username) {
    const re = /^[a-zA-Z0-9_]+$/;
    return re.test(username) && username.length >= 3;
}
