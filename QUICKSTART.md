# Quick Start Guide - Library Management System API

## 🚀 Get Started in 3 Steps

### Step 1: Build the Project
```bash
make
```

### Step 2: Start the Server

**Choose one option:**

#### Option A: Simple Run (for testing)
```bash
./build/http_api_server
```

#### Option B: With Supervisor (recommended for production)

**Linux/WSL:**
```bash
chmod +x supervisor.sh
./supervisor.sh
```

**Windows (PowerShell):**
```powershell
.\supervisor.ps1
```

**Windows (Batch):**
```cmd
supervisor.bat
```

### Step 3: Access the API

Open your browser and visit:
- **API Documentation:** http://localhost:8080/api/v1/docs
- **Dashboard:** http://localhost:8080/api/v1/dashboard
- **All Books:** http://localhost:8080/api/v1/books

## 📚 Common Operations

### View All Books
```bash
curl http://localhost:8080/api/v1/books
```

### Search Books
```bash
curl "http://localhost:8080/api/v1/books/search?query=gatsby"
```

### Create a New User
```bash
curl -X POST http://localhost:8080/api/v1/users \
  -H "Content-Type: application/json" \
  -d '{
    "userId": 100,
    "name": "John Doe",
    "email": "john@example.com",
    "membershipType": "Student"
  }'
```

### Borrow a Book
```bash
curl -X POST http://localhost:8080/api/v1/borrow \
  -H "Content-Type: application/json" \
  -d '{
    "userId": 100,
    "bookId": 101
  }'
```

### Return a Book
```bash
curl -X POST http://localhost:8080/api/v1/return \
  -H "Content-Type: application/json" \
  -d '{
    "userId": 100,
    "bookId": 101
  }'
```

### Get Statistics
```bash
# Dashboard overview
curl http://localhost:8080/api/v1/dashboard

# Most borrowed books
curl http://localhost:8080/api/v1/statistics/most-borrowed

# Most active users
curl http://localhost:8080/api/v1/statistics/most-active
```

## 🎯 Using the Interactive API Documentation

1. Start the server
2. Open http://localhost:8080/api/v1/docs in your browser
3. Find the endpoint you want to test
4. Click "Try it out"
5. Fill in any required parameters or request body
6. Click "Send Request"
7. View the response

## 🔧 Troubleshooting

### Port Already in Use
```bash
# Linux/Mac - Find and kill process on port 8080
lsof -i :8080
kill -9 <PID>

# Windows - Find and kill process
netstat -ano | findstr :8080
taskkill /PID <PID> /F
```

### Build Errors
```bash
# Clean and rebuild
make clean
make
```

### Server Crashes
- Check logs in `logs/api_server_error.log`
- The supervisor will automatically restart the server
- Check for port conflicts or missing files

## 📁 Important Files

- `build/http_api_server` - The compiled server executable
- `library_data.json` - Database (created automatically)
- `logs/` - Server logs (created by supervisor)
- `backend/api-docs.html` - API documentation source

## 🛑 Stopping the Server

**Direct Run:** Press `Ctrl+C`

**With Supervisor:** Press `Ctrl+C` (stops both supervisor and server)

## 💡 Tips

1. **Use the supervisor in production** - it will restart the server if it crashes
2. **Check the logs** - all logs are in the `logs/` directory
3. **Use the interactive docs** - easier than curl for testing
4. **Data persists** - all changes are saved to `library_data.json`

## 🎓 Next Steps

- Explore all endpoints in the API documentation
- Integrate with the frontend in `frontend/` directory
- Customize the supervisor settings if needed
- Check the full README.md for advanced configuration

---

**Need Help?** Check the logs in `logs/` or review the full README.md
