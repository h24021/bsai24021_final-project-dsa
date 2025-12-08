# Development Roadmap & Tasks

## Current Status: ✅ Foundation Complete

The project has been refactored with a professional structure. Below is the roadmap for continued development.

---

## 🎯 Phase 1: Core Features Enhancement (1-2 weeks)

### Models Enhancement
- [ ] Add `Author` and `Category` as separate classes
- [ ] Add `Transaction` model for borrow/return records
- [ ] Add `Review` model for book reviews
- [ ] Implement proper getters and setters for all fields
- [ ] Add validation methods

### Library Service Improvements
- [ ] Implement search by title
- [ ] Implement search by author
- [ ] Implement search by category
- [ ] Add update book functionality
- [ ] Add delete book functionality
- [ ] Add update user functionality
- [ ] Add delete user functionality
- [ ] Implement borrow book logic
- [ ] Implement return book logic
- [ ] Add late fee calculation
- [ ] Add due date tracking

### File Persistence
- [ ] Implement save to file (books.txt, users.txt)
- [ ] Implement load from file
- [ ] Use CSV or JSON format
- [ ] Auto-save on changes

---

## 🌐 Phase 2: REST API Implementation (2-3 weeks)

### HTTP Server Setup
- [ ] Choose library (cpp-httplib recommended)
- [ ] Install dependencies
- [ ] Create server wrapper class
- [ ] Configure CORS
- [ ] Set up error handling

### Book Endpoints
- [ ] `GET /api/books` - Get all books
- [ ] `GET /api/books/:id` - Get book by ID
- [ ] `GET /api/books/isbn/:isbn` - Get book by ISBN
- [ ] `POST /api/books` - Create book
- [ ] `PUT /api/books/:id` - Update book
- [ ] `DELETE /api/books/:id` - Delete book
- [ ] `GET /api/books/search?q=query` - Search books

### User Endpoints
- [ ] `GET /api/users` - Get all users
- [ ] `GET /api/users/:id` - Get user by ID
- [ ] `POST /api/users` - Create user
- [ ] `PUT /api/users/:id` - Update user
- [ ] `DELETE /api/users/:id` - Delete user
- [ ] `POST /api/users/:id/borrow` - Borrow book
- [ ] `POST /api/users/:id/return` - Return book
- [ ] `GET /api/users/:id/history` - Borrowing history

### JSON Integration
- [ ] Install nlohmann/json library
- [ ] Implement JSON serialization for Book
- [ ] Implement JSON serialization for User
- [ ] Implement JSON deserialization
- [ ] Add JSON validation

### Testing
- [ ] Test all endpoints with Postman
- [ ] Create API test collection
- [ ] Document expected responses
- [ ] Handle edge cases

---

## 💾 Phase 3: Database Integration (2-3 weeks)

### Database Setup
- [ ] Choose database (SQLite for dev)
- [ ] Install database driver
- [ ] Create database schema
- [ ] Design ER diagram
- [ ] Create migration scripts

### Repository Pattern
- [ ] Create `IRepository` interface
- [ ] Implement `BookRepository`
- [ ] Implement `UserRepository`
- [ ] Implement `TransactionRepository`
- [ ] Add connection pooling

### Database Operations
- [ ] CRUD operations for books
- [ ] CRUD operations for users
- [ ] Transaction logging
- [ ] Query optimization
- [ ] Add indexes

### Migration System
- [ ] Create migration framework
- [ ] Version 1: Initial schema
- [ ] Version 2: Add indexes
- [ ] Rollback support

---

## 🔐 Phase 4: Authentication & Security (1-2 weeks)

### Authentication
- [ ] Install JWT library
- [ ] Implement user registration
- [ ] Implement user login
- [ ] Generate JWT tokens
- [ ] Token validation middleware
- [ ] Refresh token logic

### Authorization
- [ ] Define user roles (Admin, Librarian, Student)
- [ ] Implement RBAC (Role-Based Access Control)
- [ ] Protect admin endpoints
- [ ] Protect user-specific endpoints
- [ ] Add permission checks

### Security
- [ ] Hash passwords (bcrypt)
- [ ] Validate all inputs
- [ ] Prevent SQL injection
- [ ] Add rate limiting
- [ ] Implement HTTPS (production)
- [ ] Add CORS configuration

---

## 🎨 Phase 5: Frontend Development (3-4 weeks)

### Setup
- [ ] Choose framework (React + Vite recommended)
- [ ] Initialize project
- [ ] Install dependencies
- [ ] Setup routing
- [ ] Configure API client

### Pages
- [ ] Home/Landing page
- [ ] Login page
- [ ] Registration page
- [ ] Dashboard
- [ ] Book catalog page
- [ ] Book details page
- [ ] User profile page
- [ ] Admin panel
- [ ] Search results page

### Components
- [ ] Header/Navigation
- [ ] Footer
- [ ] Book card component
- [ ] Search bar
- [ ] Filter sidebar
- [ ] Pagination
- [ ] Loading spinner
- [ ] Error boundaries

### Features
- [ ] Book browsing
- [ ] Advanced search
- [ ] Filters (category, author, availability)
- [ ] Borrow request
- [ ] Return book
- [ ] View history
- [ ] Admin CRUD operations
- [ ] User management (admin)
- [ ] Dashboard statistics

### Styling
- [ ] Choose UI library (Material-UI, Chakra, Tailwind)
- [ ] Responsive design
- [ ] Dark mode support
- [ ] Accessibility (WCAG)

---

## 🧪 Phase 6: Testing (Ongoing)

### Unit Tests
- [ ] Setup Google Test
- [ ] Test Book model
- [ ] Test User model
- [ ] Test Library service
- [ ] Test controllers
- [ ] Achieve 80%+ coverage

### Integration Tests
- [ ] Test API endpoints
- [ ] Test database operations
- [ ] Test authentication flow
- [ ] Test authorization

### End-to-End Tests
- [ ] Setup Selenium/Playwright
- [ ] Test user workflows
- [ ] Test admin workflows
- [ ] Test error scenarios

---

## 📦 Phase 7: DevOps & Deployment (1-2 weeks)

### Docker
- [ ] Create Dockerfile for backend
- [ ] Create Dockerfile for frontend
- [ ] Create docker-compose.yml
- [ ] Setup database container
- [ ] Setup nginx container

### CI/CD
- [ ] Setup GitHub Actions
- [ ] Automated testing
- [ ] Automated builds
- [ ] Automated deployment
- [ ] Code quality checks (linting)

### Monitoring
- [ ] Add logging (spdlog)
- [ ] Error tracking
- [ ] Performance monitoring
- [ ] Health check endpoint

### Deployment
- [ ] Deploy to cloud (AWS, Azure, Heroku)
- [ ] Setup domain
- [ ] Configure SSL/TLS
- [ ] Setup backup strategy
- [ ] Create deployment docs

---

## 🚀 Phase 8: Advanced Features (Future)

### Advanced Search
- [ ] Full-text search
- [ ] Elasticsearch integration
- [ ] Search suggestions
- [ ] Fuzzy matching

### Notifications
- [ ] Email notifications
- [ ] SMS notifications
- [ ] Push notifications
- [ ] Due date reminders

### Analytics
- [ ] Popular books report
- [ ] Usage statistics
- [ ] User activity tracking
- [ ] Admin dashboard charts

### Mobile App
- [ ] React Native app
- [ ] Flutter app
- [ ] iOS app
- [ ] Android app

### AI Features
- [ ] Book recommendations
- [ ] Smart search
- [ ] Auto-categorization
- [ ] Reading predictions

---

## 📋 Immediate Next Steps (This Week)

1. **Complete Library Service**
   ```bash
   # Implement in backend/src/services/Library.cpp
   - searchBookByTitle()
   - searchBookByAuthor()
   - updateBook()
   - deleteBook()
   - borrowBook()
   - returnBook()
   ```

2. **Add File Persistence**
   ```bash
   # Create backend/src/services/FileStorage.cpp
   - saveBooks()
   - loadBooks()
   - saveUsers()
   - loadUsers()
   ```

3. **Install HTTP Library**
   ```bash
   # Option 1: cpp-httplib (header-only)
   curl -O https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
   mv httplib.h backend/include/
   
   # Option 2: Crow
   # Follow installation guide
   ```

4. **Test API Locally**
   ```bash
   # Start server
   ./build/library_system
   
   # Test with curl
   curl http://localhost:8080/api/books
   ```

---

## 📚 Learning Resources

- **C++ HTTP Libraries**: cpp-httplib, Crow, Drogon
- **JSON Library**: nlohmann/json
- **Database**: SQLite, PostgreSQL
- **Testing**: Google Test, Catch2
- **Frontend**: React, Vue, Vite
- **Docker**: Official Docker docs
- **CI/CD**: GitHub Actions docs

---

**Last Updated**: December 2024  
**Current Phase**: Phase 1 - Core Features  
**Estimated Total Time**: 12-16 weeks for full implementation
