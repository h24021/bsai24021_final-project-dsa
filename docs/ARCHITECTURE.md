# Library Management System Architecture

## Overview

This document describes the architecture and design of the Library Management System, a modular C++ application designed to support API integration and frontend development.

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                         Frontend                             │
│              (React/Vue - Future Implementation)             │
└───────────────────────────┬─────────────────────────────────┘
                            │
                    HTTP/REST API
                            │
┌───────────────────────────▼─────────────────────────────────┐
│                      API Layer                               │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              Router (Request Handler)                 │   │
│  └────────────────────┬─────────────────────────────────┘   │
└───────────────────────┼─────────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────────┐
│                  Controller Layer                            │
│  ┌──────────────────┐         ┌─────────────────────┐       │
│  │ BookController   │         │  UserController     │       │
│  │ - getAllBooks()  │         │  - getAllUsers()    │       │
│  │ - getBookById()  │         │  - getUserById()    │       │
│  │ - createBook()   │         │  - createUser()     │       │
│  │ - updateBook()   │         │  - borrowBook()     │       │
│  │ - deleteBook()   │         │  - returnBook()     │       │
│  └────────┬─────────┘         └──────────┬──────────┘       │
└───────────┼────────────────────────────────┼─────────────────┘
            │                                │
┌───────────▼────────────────────────────────▼─────────────────┐
│                   Service Layer                               │
│  ┌──────────────────────────────────────────────────────┐    │
│  │              Library Service                          │    │
│  │  - Business Logic                                     │    │
│  │  - Data Validation                                    │    │
│  │  - Transaction Management                             │    │
│  └────────────────────┬─────────────────────────────────┘    │
└───────────────────────┼──────────────────────────────────────┘
                        │
┌───────────────────────▼──────────────────────────────────────┐
│                    Model Layer                                │
│  ┌──────────────┐              ┌──────────────┐              │
│  │     Book     │              │     User     │              │
│  │  - bookID    │              │  - userID    │              │
│  │  - title     │              │  - name      │              │
│  │  - author    │              │  - role      │              │
│  │  - isbn      │              │  - borrowed  │              │
│  └──────────────┘              └──────────────┘              │
└───────────────────────────────────────────────────────────────┘
```

## Design Patterns

### 1. **Layered Architecture**
- **Presentation Layer**: Future frontend (React/Vue)
- **API Layer**: REST endpoint handlers and routing
- **Business Logic Layer**: Services containing core logic
- **Data Layer**: Models representing entities

### 2. **Model-View-Controller (MVC)**
- **Models**: Data structures (Book, User)
- **Controllers**: Handle requests, return responses
- **Views**: Future frontend components

### 3. **Dependency Injection**
Controllers receive Library service instance via constructor injection.

```cpp
BookController bookCtrl(&library);
```

### 4. **Repository Pattern** (Future)
When database integration is added, repositories will abstract data access:

```cpp
class IBookRepository {
    virtual vector<Book> findAll() = 0;
    virtual Book findById(int id) = 0;
    virtual void save(Book book) = 0;
};
```

## Directory Structure Rationale

### `/backend`
Contains all server-side code, separating it from future frontend code.

### `/backend/include`
Header files organized by responsibility:
- **models/**: Data structures
- **services/**: Business logic interfaces
- **controllers/**: API endpoint handlers

### `/backend/src`
Implementation files mirroring the include structure.

### `/build`
Generated build artifacts (excluded from version control).

### `/config`
Configuration files for different environments.

### `/docs`
Project documentation.

### `/tests`
Test suites organized by layer.

### `/frontend`
Future frontend application.

## Data Flow

### Example: Create Book Request

```
1. Frontend sends POST request
   └─> /api/books with JSON body

2. Router receives request
   └─> Matches route and calls BookController::createBook()

3. BookController processes request
   ├─> Validates input data
   ├─> Calls Library::addBook()
   └─> Returns JSON response

4. Library Service (Business Logic)
   ├─> Validates business rules
   ├─> Creates Book model
   └─> Stores in collection (future: database)

5. Response flows back to frontend
   └─> JSON response with created book data
```

## Key Design Decisions

### 1. **Separation of Concerns**
Each layer has a single responsibility:
- Models: Data structure
- Services: Business logic
- Controllers: Request handling
- API: Routing and HTTP

### 2. **API-First Design**
Controllers are designed to return JSON strings, making it easy to integrate with HTTP libraries like cpp-httplib or Crow.

### 3. **Const Correctness**
Getter methods marked as `const` to enable usage with const references and improve code safety.

### 4. **Forward Compatibility**
Structure supports future enhancements:
- Database integration
- Authentication/Authorization
- Caching layer
- Message queues
- Microservices

## Future Enhancements

### Phase 1: HTTP Server Integration
```cpp
#include "httplib.h"

httplib::Server svr;
BookController bookCtrl(&library);

svr.Post("/api/books", [&](const auto& req, auto& res) {
    // Parse JSON from req.body
    // Call bookCtrl.createBook(...)
    // Return JSON response
});
```

### Phase 2: Database Layer
```
┌─────────────────────────┐
│    Service Layer        │
└───────────┬─────────────┘
            │
┌───────────▼─────────────┐
│  Repository Layer       │
│  - BookRepository       │
│  - UserRepository       │
└───────────┬─────────────┘
            │
┌───────────▼─────────────┐
│    Database             │
│  SQLite / PostgreSQL    │
└─────────────────────────┘
```

### Phase 3: Authentication Middleware
```cpp
class AuthMiddleware {
    bool verify(string token);
    User getCurrentUser(string token);
};
```

### Phase 4: Microservices (Optional)
- Book Service
- User Service
- Transaction Service
- Notification Service

## Technology Stack

### Current
- C++14
- Standard Library (STL)
- Make/CMake

### Planned
- **HTTP Server**: cpp-httplib or Crow
- **Database**: SQLite (dev), PostgreSQL (prod)
- **JSON**: nlohmann/json
- **Testing**: Google Test
- **Logging**: spdlog
- **Frontend**: React/Vue + TypeScript

## Performance Considerations

1. **In-Memory Storage**: Current implementation uses vectors
2. **Future Database**: Will add indexing on ISBN, ID
3. **Caching**: Consider Redis for frequently accessed data
4. **Connection Pooling**: For database connections
5. **Async Operations**: Consider async I/O for HTTP

## Security Considerations

1. **Input Validation**: Validate all user inputs
2. **SQL Injection**: Use prepared statements
3. **Authentication**: JWT-based authentication
4. **Authorization**: Role-based access control (RBAC)
5. **HTTPS**: SSL/TLS for production
6. **CORS**: Properly configured for frontend

## Scalability

### Horizontal Scaling
- Stateless API design
- Load balancer (nginx)
- Multiple API instances

### Vertical Scaling
- Optimize database queries
- Add database indexes
- Implement caching

---

**Last Updated**: December 2024  
**Version**: 1.0
