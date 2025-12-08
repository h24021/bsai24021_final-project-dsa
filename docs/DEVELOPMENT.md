# Development Guidelines

## Code Style

### C++ Conventions
- Use PascalCase for class names: `BookController`, `LibraryService`
- Use camelCase for functions and variables: `addBook()`, `getUserById()`
- Use UPPER_CASE for constants: `MAX_BOOKS`, `DEFAULT_TIMEOUT`
- Always use `#pragma once` for header guards
- Keep header files (.h) in `backend/include/`
- Keep implementation files (.cpp) in `backend/src/`

### File Organization
```
Class Declaration → Header file (.h) in include/
Class Implementation → Source file (.cpp) in src/
```

## Building

### Make Commands
```bash
make          # Build project
make run      # Run executable
make clean    # Clean build artifacts
make help     # Show all commands
```

### CMake Commands
```bash
cmake -S . -B build          # Configure
cmake --build build          # Build
cmake --build build --clean  # Clean
```

## Git Workflow

### Branching Strategy
- `main` - Production-ready code
- `develop` - Integration branch
- `feature/*` - New features
- `bugfix/*` - Bug fixes
- `hotfix/*` - Urgent fixes

### Commit Messages
Follow conventional commits:
```
feat: add book borrowing API endpoint
fix: resolve memory leak in Library class
docs: update API documentation
refactor: reorganize controller structure
test: add unit tests for User model
```

### Before Committing
1. Build successfully: `make clean && make`
2. Run tests: `make test`
3. Check formatting
4. Update documentation if needed

## Adding New Features

### 1. Create Model
```cpp
// backend/include/models/YourModel.h
#pragma once
class YourModel {
    // Define your model
};
```

### 2. Create Service
```cpp
// backend/include/services/YourService.h
#pragma once
class YourService {
    // Business logic
};
```

### 3. Create Controller
```cpp
// backend/include/controllers/YourController.h
#pragma once
class YourController {
    // API endpoints
};
```

### 4. Update Build System
Add to Makefile:
```makefile
YOUR_SRCS = $(SRC_DIR)/models/YourModel.cpp \
            $(SRC_DIR)/services/YourService.cpp \
            $(SRC_DIR)/controllers/YourController.cpp
```

## Testing

### Unit Tests Structure
```
tests/
├── models/
│   ├── test_book.cpp
│   └── test_user.cpp
├── services/
│   └── test_library.cpp
└── controllers/
    └── test_book_controller.cpp
```

### Running Tests
```bash
# With Make
make test

# With CMake
cd build && ctest
```

## API Integration

### Adding HTTP Server (Future)
Recommended libraries:
1. **cpp-httplib** - Simple and lightweight
2. **Crow** - Fast and easy to use
3. **Drogon** - High performance

### Example with cpp-httplib
```cpp
#include "httplib.h"

httplib::Server svr;
BookController bookCtrl(&library);

svr.Get("/api/books", [&](const auto& req, auto& res) {
    res.set_content(bookCtrl.getAllBooks(), "application/json");
});

svr.listen("0.0.0.0", 8080);
```

## Database Integration

### Recommended Libraries
- **SQLite**: sqlite3, SQLiteCpp
- **PostgreSQL**: libpqxx
- **MySQL**: mysql-connector-cpp

### Migration Strategy
1. Create migration files in `backend/migrations/`
2. Version migrations: `001_initial_schema.sql`
3. Track applied migrations

## Frontend Integration

### Recommended Stack
- **React** + TypeScript + Vite
- **Vue 3** + TypeScript + Vite
- **Svelte** + TypeScript

### API Client
```typescript
// frontend/src/api/books.ts
export async function getAllBooks() {
    const response = await fetch('http://localhost:8080/api/books');
    return response.json();
}
```

## Docker Setup (Future)

```dockerfile
FROM gcc:latest
WORKDIR /app
COPY . .
RUN make
CMD ["./build/library_system"]
```

## Deployment Checklist

- [ ] All tests passing
- [ ] Documentation updated
- [ ] Environment variables configured
- [ ] Database migrations applied
- [ ] Build successful in production mode
- [ ] API endpoints tested
- [ ] Security review completed
- [ ] Performance optimization done

## Resources

- [C++ Reference](https://en.cppreference.com/)
- [REST API Best Practices](https://restfulapi.net/)
- [Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
