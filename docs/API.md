# API Documentation

## Base URL
```
http://localhost:8080/api
```

## Response Format

All API responses follow this JSON structure:

### Success Response
```json
{
  "status": "success",
  "data": { ... },
  "message": "Operation successful"
}
```

### Error Response
```json
{
  "status": "error",
  "code": 404,
  "message": "Resource not found"
}
```

## Books API

### Get All Books
- **Endpoint**: `GET /api/books`
- **Description**: Retrieve all books in the library
- **Response**:
```json
{
  "status": "success",
  "data": [
    {
      "bookID": 1,
      "title": "The Great Gatsby",
      "author": "F. Scott Fitzgerald",
      "isbn": "9780743273565",
      "category": "Fiction",
      "copies": 3,
      "availableCopies": 3
    }
  ]
}
```

### Get Book by ID
- **Endpoint**: `GET /api/books/:id`
- **Parameters**: 
  - `id` (path) - Book ID
- **Response**: Single book object

### Get Book by ISBN
- **Endpoint**: `GET /api/books/isbn/:isbn`
- **Parameters**:
  - `isbn` (path) - Book ISBN
- **Response**: Single book object

### Create Book
- **Endpoint**: `POST /api/books`
- **Body**:
```json
{
  "title": "1984",
  "author": "George Orwell",
  "isbn": "9780451524935",
  "category": "Fiction",
  "copies": 5,
  "availableCopies": 5
}
```
- **Response**: Created book object with ID

### Update Book
- **Endpoint**: `PUT /api/books/:id`
- **Parameters**:
  - `id` (path) - Book ID
- **Body**: Same as Create Book
- **Response**: Updated book object

### Delete Book
- **Endpoint**: `DELETE /api/books/:id`
- **Parameters**:
  - `id` (path) - Book ID
- **Response**: Success message

## Users API

### Get All Users
- **Endpoint**: `GET /api/users`
- **Description**: Retrieve all users
- **Response**:
```json
{
  "status": "success",
  "data": [
    {
      "userID": 1,
      "name": "Ali Khan",
      "role": "Student",
      "borrowedBooks": 0
    }
  ]
}
```

### Get User by ID
- **Endpoint**: `GET /api/users/:id`
- **Parameters**:
  - `id` (path) - User ID
- **Response**: Single user object

### Create User
- **Endpoint**: `POST /api/users`
- **Body**:
```json
{
  "name": "Sara Ahmed",
  "role": "Librarian"
}
```
- **Response**: Created user object with ID

### Update User
- **Endpoint**: `PUT /api/users/:id`
- **Parameters**:
  - `id` (path) - User ID
- **Body**: Same as Create User
- **Response**: Updated user object

### Delete User
- **Endpoint**: `DELETE /api/users/:id`
- **Parameters**:
  - `id` (path) - User ID
- **Response**: Success message

### Borrow Book
- **Endpoint**: `POST /api/users/:id/borrow`
- **Parameters**:
  - `id` (path) - User ID
- **Body**:
```json
{
  "bookID": 1
}
```
- **Response**: Success message

### Return Book
- **Endpoint**: `POST /api/users/:id/return`
- **Parameters**:
  - `id` (path) - User ID
- **Body**:
```json
{
  "bookID": 1
}
```
- **Response**: Success message

## Error Codes

| Code | Description |
|------|-------------|
| 200  | Success |
| 201  | Created |
| 400  | Bad Request |
| 401  | Unauthorized |
| 403  | Forbidden |
| 404  | Not Found |
| 500  | Internal Server Error |

## Authentication

(To be implemented)

Future versions will require authentication:
```
Authorization: Bearer <token>
```

## Rate Limiting

(To be implemented)

## Pagination

For list endpoints, pagination will be supported:
```
GET /api/books?page=1&limit=10
```
