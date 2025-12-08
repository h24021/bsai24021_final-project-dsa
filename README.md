# Library Management System

A comprehensive library management system built with C++ featuring a modular architecture designed for API integration and future frontend development.

## 📁 Project Structure

```
.
├── backend/                    # Backend application code
│   ├── include/               # Header files
│   │   ├── models/           # Data models (Book, User)
│   │   ├── services/         # Business logic (Library service)
│   │   └── controllers/      # API controllers (REST handlers)
│   └── src/                  # Implementation files
│       ├── models/           # Model implementations
│       ├── services/         # Service implementations
│       ├── controllers/      # Controller implementations
│       ├── api/              # API routing and middleware
│       └── main.cpp          # Application entry point
├── build/                     # Build output directory
├── config/                    # Configuration files
├── docs/                      # Documentation
├── tests/                     # Unit and integration tests
├── frontend/                  # Frontend application (future)
├── CMakeLists.txt            # CMake build configuration
├── Makefile                  # Make build configuration
└── .gitignore                # Git ignore rules
```

## 🚀 Features

- **Book Management**: Add, search, update, and delete books
- **User Management**: Manage library users and roles
- **Modular Architecture**: Separation of concerns (Models, Services, Controllers)
- **API-Ready**: Controller layer designed for REST API integration
- **Build Flexibility**: Support for both Make and CMake

## 🛠️ Building the Project

### Using Make (Recommended)

```bash
# Build the project
make

# Build and run
make build-run

# Clean build artifacts
make clean

# View all available commands
make help
```

### Using CMake

```bash
# Create build directory
mkdir -p build && cd build

# Configure and build
cmake ..
make

# Run the executable
./bin/library_system
```

## 📖 Usage

### Current CLI Mode

```cpp
Library lib;

// Add books
lib.addBook(Book(1, "The Great Gatsby", "F. Scott Fitzgerald", 
                  "9780743273565", "Fiction", 3, 3));

// Add users
lib.addUser(User(1, "Ali Khan", "Student"));

// Search books
lib.searchBookByISBN("9780743273565");
```

### Future API Mode

The system is structured to support RESTful API endpoints:

```
GET    /api/books              - Get all books
GET    /api/books/:id          - Get book by ID
GET    /api/books/isbn/:isbn   - Get book by ISBN
POST   /api/books              - Create new book
PUT    /api/books/:id          - Update book
DELETE /api/books/:id          - Delete book

GET    /api/users              - Get all users
GET    /api/users/:id          - Get user by ID
POST   /api/users              - Create new user
POST   /api/users/:id/borrow   - Borrow a book
POST   /api/users/:id/return   - Return a book
```

## 🏗️ Architecture

### Layered Architecture

1. **Models Layer** (`backend/include/models/`)
   - Data structures and entities
   - `Book.h` - Book entity
   - `User.h` - User entity

2. **Services Layer** (`backend/include/services/`)
   - Business logic and data management
   - `Library.h` - Core library operations

3. **Controllers Layer** (`backend/include/controllers/`)
   - API request handlers
   - `BookController.h` - Book API endpoints
   - `UserController.h` - User API endpoints

4. **API Layer** (`backend/src/api/`)
   - Routing and middleware
   - `Router.cpp` - Request routing

## 🔧 Configuration

Configuration files can be placed in the `config/` directory:
- Database connection settings
- API server configuration
- Authentication settings
- Logging configuration

## 📚 API Documentation

Detailed API documentation will be available in the `docs/` directory:
- `docs/API.md` - API endpoint specifications
- `docs/ARCHITECTURE.md` - System architecture guide
- `docs/DEVELOPMENT.md` - Development guidelines

## 🧪 Testing

```bash
# Run tests (future implementation)
make test

# Or with CMake
cd build && ctest
```

## 🔄 Development Roadmap

- [x] Core data models (Book, User)
- [x] Library service implementation
- [x] Modular project structure
- [x] API controller layer
- [x] Build system (Make & CMake)
- [ ] RESTful API with HTTP server (cpp-httplib or Crow)
- [ ] Database integration (SQLite/PostgreSQL)
- [ ] Authentication & authorization
- [ ] Frontend application (React/Vue)
- [ ] Comprehensive testing suite
- [ ] Docker containerization
- [ ] CI/CD pipeline

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is part of a DSA Final Project.

## 👥 Authors

- DSA Final Project Team

## 🙏 Acknowledgments

- Data Structures and Algorithms Course
- C++ Standard Library
- Future: cpp-httplib, Crow, or similar HTTP library

---

**Note**: This project is currently in active development. The API layer is structured but requires HTTP server integration. Frontend integration will be implemented in future iterations.
