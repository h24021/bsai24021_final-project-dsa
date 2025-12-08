# Library Management System - REST API Documentation

## Base URL
```
http://localhost:8080/api/v1
```

---

## 📚 **Books API**

### 1. Get All Books
**GET** `/books`

Returns all books in the library.

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "id": 1,
      "title": "1984",
      "author": "George Orwell",
      "isbn": "9780451524935",
      "category": "Dystopian",
      "copies": 5,
      "availableCopies": 3
    }
  ],
  "count": 1
}
```

---

### 2. Get Book by ID
**GET** `/books/:id`

**Response:**
```json
{
  "status": "success",
  "data": {
    "id": 1,
    "title": "1984",
    "author": "George Orwell",
    "isbn": "9780451524935",
    "category": "Dystopian",
    "copies": 5,
    "availableCopies": 3
  }
}
```

---

### 3. Search Books
**GET** `/books/search?title=1984&author=orwell&category=fiction`

Query Parameters:
- `title` - Search by title (case-insensitive, partial match)
- `author` - Search by author (case-insensitive, partial match)
- `category` - Search by category (case-insensitive)

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "id": 1,
      "title": "1984",
      "author": "George Orwell",
      "isbn": "9780451524935",
      "category": "Dystopian",
      "copies": 5,
      "availableCopies": 3
    }
  ],
  "count": 1,
  "query": {
    "title": "1984",
    "author": "orwell"
  }
}
```

---

### 4. Add New Book
**POST** `/books`

**Request Body:**
```json
{
  "title": "The Great Gatsby",
  "author": "F. Scott Fitzgerald",
  "isbn": "9780743273565",
  "category": "Fiction",
  "copies": 3
}
```

**Response:**
```json
{
  "status": "success",
  "message": "Book added successfully",
  "data": {
    "id": 10,
    "title": "The Great Gatsby",
    "author": "F. Scott Fitzgerald",
    "isbn": "9780743273565",
    "category": "Fiction",
    "copies": 3,
    "availableCopies": 3
  }
}
```

---

### 5. Update Book
**PUT** `/books/:id`

**Request Body:**
```json
{
  "copies": 5,
  "category": "Classic Fiction"
}
```

**Response:**
```json
{
  "status": "success",
  "message": "Book updated successfully",
  "data": {
    "id": 10,
    "title": "The Great Gatsby",
    "author": "F. Scott Fitzgerald",
    "isbn": "9780743273565",
    "category": "Classic Fiction",
    "copies": 5,
    "availableCopies": 3
  }
}
```

---

### 6. Delete Book
**DELETE** `/books/:id`

**Response:**
```json
{
  "status": "success",
  "message": "Book deleted successfully"
}
```

---

## 👥 **Users API**

### 1. Get All Users
**GET** `/users`

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "id": 101,
      "name": "Alice Smith",
      "email": "alice@example.com",
      "role": "Student",
      "borrowedBooks": 2,
      "borrowedBookIds": [1, 3]
    }
  ],
  "count": 1
}
```

---

### 2. Get User by ID
**GET** `/users/:id`

**Response:**
```json
{
  "status": "success",
  "data": {
    "id": 101,
    "name": "Alice Smith",
    "email": "alice@example.com",
    "role": "Student",
    "borrowedBooks": 2,
    "borrowedBookIds": [1, 3]
  }
}
```

---

### 3. Get User by Email
**GET** `/users/email/:email`

**Response:**
```json
{
  "status": "success",
  "data": {
    "id": 101,
    "name": "Alice Smith",
    "email": "alice@example.com",
    "role": "Student",
    "borrowedBooks": 2,
    "borrowedBookIds": [1, 3]
  }
}
```

---

### 4. Register New User
**POST** `/users`

**Request Body:**
```json
{
  "name": "Bob Jones",
  "email": "bob@example.com",
  "role": "Faculty"
}
```

**Response:**
```json
{
  "status": "success",
  "message": "User registered successfully",
  "data": {
    "id": 102,
    "name": "Bob Jones",
    "email": "bob@example.com",
    "role": "Faculty",
    "borrowedBooks": 0,
    "borrowedBookIds": []
  }
}
```

---

### 5. Update User
**PUT** `/users/:id`

**Request Body:**
```json
{
  "name": "Robert Jones",
  "role": "Professor"
}
```

**Response:**
```json
{
  "status": "success",
  "message": "User updated successfully",
  "data": {
    "id": 102,
    "name": "Robert Jones",
    "email": "bob@example.com",
    "role": "Professor",
    "borrowedBooks": 0,
    "borrowedBookIds": []
  }
}
```

---

### 6. Delete User
**DELETE** `/users/:id`

**Response:**
```json
{
  "status": "success",
  "message": "User deleted successfully"
}
```

---

## 📖 **Borrow/Return API**

### 1. Borrow Book
**POST** `/borrow`

**Request Body:**
```json
{
  "userId": 101,
  "bookId": 1
}
```

**Response:**
```json
{
  "status": "success",
  "message": "Book borrowed successfully",
  "data": {
    "borrowId": "BRW-2024-001",
    "userId": 101,
    "userName": "Alice Smith",
    "bookId": 1,
    "bookTitle": "1984",
    "borrowDate": "2024-12-08T10:30:00Z",
    "dueDate": "2024-12-22T10:30:00Z"
  }
}
```

**Error Response:**
```json
{
  "status": "error",
  "message": "Book not available",
  "code": "BOOK_UNAVAILABLE"
}
```

---

### 2. Return Book
**POST** `/return`

**Request Body:**
```json
{
  "userId": 101,
  "bookId": 1
}
```

**Response:**
```json
{
  "status": "success",
  "message": "Book returned successfully",
  "data": {
    "borrowId": "BRW-2024-001",
    "userId": 101,
    "bookId": 1,
    "borrowDate": "2024-12-08T10:30:00Z",
    "returnDate": "2024-12-15T14:20:00Z",
    "daysHeld": 7
  }
}
```

---

### 3. Get User's Borrowed Books
**GET** `/users/:id/borrowed`

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "bookId": 1,
      "title": "1984",
      "author": "George Orwell",
      "borrowDate": "2024-12-08T10:30:00Z",
      "dueDate": "2024-12-22T10:30:00Z",
      "daysRemaining": 7
    }
  ],
  "count": 1
}
```

---

### 4. Get Book's Borrow History
**GET** `/books/:id/history`

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "borrowId": "BRW-2024-001",
      "userId": 101,
      "userName": "Alice Smith",
      "borrowDate": "2024-12-08T10:30:00Z",
      "returnDate": "2024-12-15T14:20:00Z",
      "daysHeld": 7
    }
  ],
  "count": 1
}
```

---

## 📊 **Statistics & Dashboard API**

### 1. Get Dashboard Overview
**GET** `/dashboard`

**Response:**
```json
{
  "status": "success",
  "data": {
    "totalBooks": 150,
    "totalUsers": 45,
    "activeLoans": 23,
    "overdueBooks": 3,
    "popularCategories": [
      {"category": "Fiction", "count": 45},
      {"category": "Science", "count": 32}
    ]
  }
}
```

---

### 2. Get Most Borrowed Books
**GET** `/statistics/most-borrowed?limit=10`

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "bookId": 1,
      "title": "1984",
      "author": "George Orwell",
      "borrowCount": 25,
      "category": "Dystopian"
    },
    {
      "bookId": 3,
      "title": "The Great Gatsby",
      "author": "F. Scott Fitzgerald",
      "borrowCount": 18,
      "category": "Fiction"
    }
  ],
  "count": 2
}
```

---

### 3. Get Most Active Users
**GET** `/statistics/most-active?limit=10`

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "userId": 101,
      "name": "Alice Smith",
      "email": "alice@example.com",
      "borrowCount": 15,
      "role": "Student"
    },
    {
      "userId": 102,
      "name": "Bob Jones",
      "email": "bob@example.com",
      "borrowCount": 12,
      "role": "Faculty"
    }
  ],
  "count": 2
}
```

---

### 4. Get Books by Category Distribution
**GET** `/statistics/category-distribution`

**Response:**
```json
{
  "status": "success",
  "data": [
    {"category": "Fiction", "count": 45, "percentage": 30},
    {"category": "Science", "count": 32, "percentage": 21.3},
    {"category": "History", "count": 28, "percentage": 18.7}
  ],
  "totalBooks": 150
}
```

---

### 5. Get User Activity Report
**GET** `/statistics/user-activity/:id?period=30days`

**Response:**
```json
{
  "status": "success",
  "data": {
    "userId": 101,
    "name": "Alice Smith",
    "period": "30days",
    "totalBorrowed": 8,
    "totalReturned": 6,
    "currentlyBorrowed": 2,
    "averageDaysHeld": 12,
    "favoriteCategories": ["Fiction", "Science"]
  }
}
```

---

## 🔍 **Advanced Search API**

### 1. Advanced Book Search
**POST** `/search/advanced`

**Request Body:**
```json
{
  "title": "the",
  "author": "orwell",
  "category": "dystopian",
  "available": true,
  "minCopies": 1
}
```

**Response:**
```json
{
  "status": "success",
  "data": [
    {
      "id": 1,
      "title": "1984",
      "author": "George Orwell",
      "isbn": "9780451524935",
      "category": "Dystopian",
      "copies": 5,
      "availableCopies": 3,
      "relevanceScore": 0.95
    }
  ],
  "count": 1,
  "filters": {
    "title": "the",
    "author": "orwell",
    "category": "dystopian",
    "available": true
  }
}
```

---

## ❌ **Error Responses**

All errors follow this format:

```json
{
  "status": "error",
  "message": "Error description",
  "code": "ERROR_CODE",
  "details": {}
}
```

### Error Codes:
- `NOT_FOUND` - Resource not found
- `INVALID_INPUT` - Invalid request data
- `BOOK_UNAVAILABLE` - No copies available
- `USER_NOT_FOUND` - User doesn't exist
- `ALREADY_BORROWED` - User already has this book
- `NOT_BORROWED` - Book not borrowed by user
- `SERVER_ERROR` - Internal server error

---

## 📝 **HTTP Status Codes**

- `200` - OK
- `201` - Created
- `400` - Bad Request
- `404` - Not Found
- `409` - Conflict
- `500` - Internal Server Error

---

## 🔐 **Authentication (Future)**

For production, add authentication headers:

```
Authorization: Bearer <token>
```

---

## 📌 **Rate Limiting (Future)**

- 100 requests per minute per IP
- 1000 requests per hour per user

---

## 🚀 **Quick Start Examples**

### Using cURL

```bash
# Get all books
curl http://localhost:8080/api/v1/books

# Search books
curl "http://localhost:8080/api/v1/books/search?title=1984"

# Add a book
curl -X POST http://localhost:8080/api/v1/books \
  -H "Content-Type: application/json" \
  -d '{"title":"1984","author":"George Orwell","isbn":"9780451524935","category":"Dystopian","copies":5}'

# Borrow a book
curl -X POST http://localhost:8080/api/v1/borrow \
  -H "Content-Type: application/json" \
  -d '{"userId":101,"bookId":1}'

# Get statistics
curl http://localhost:8080/api/v1/statistics/most-borrowed?limit=5
```

---

## 📊 **Endpoint Summary**

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/books` | Get all books |
| GET | `/books/:id` | Get book by ID |
| GET | `/books/search` | Search books |
| POST | `/books` | Add new book |
| PUT | `/books/:id` | Update book |
| DELETE | `/books/:id` | Delete book |
| GET | `/users` | Get all users |
| GET | `/users/:id` | Get user by ID |
| GET | `/users/email/:email` | Get user by email |
| POST | `/users` | Register user |
| PUT | `/users/:id` | Update user |
| DELETE | `/users/:id` | Delete user |
| POST | `/borrow` | Borrow book |
| POST | `/return` | Return book |
| GET | `/users/:id/borrowed` | Get borrowed books |
| GET | `/books/:id/history` | Get borrow history |
| GET | `/dashboard` | Dashboard overview |
| GET | `/statistics/most-borrowed` | Most borrowed books |
| GET | `/statistics/most-active` | Most active users |
| GET | `/statistics/category-distribution` | Category stats |
| POST | `/search/advanced` | Advanced search |

**Total Endpoints: 20+**
