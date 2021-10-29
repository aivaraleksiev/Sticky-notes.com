# Sticky-notes REST API documentation

This is the Sticky-notes REST API Reference. It provides API documentation, request/response samples, and usage description.

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

> The user must be able to
> - create notes with title, text and color.
> - edit/delete notes 
> - search for Notes: by title / by text / by color

## REST API

| Method                | Endpoint                                     | Description  |                             
| :------:              | ------                                       | ------       |                        
| [POST](#Request)      | [/user/signUp](#Request)                     | Create a user with username and password. |
| [POST](#Request-1)    | [/user/login](#Request-1)                    | Sign in a user. |
| [DELETE](#Request-2)  | [/user/{username}](#Request-2)               | Deletes a user. |       
| [GET](#Request-3)     | [/{username}/notes](#Request-3)              | Lists all notes created by the user. |     
| [GET](#Request-4)     | [/{username}/notes/{noteId}](#Request-4)     | List information for a note. |
| [GET](#Request-5)     | [/{username}/notes?title={string}&text={string}&color={string}](#Request-5) | Filter notes: _by title_ and/or _by text_ and/or _by color_. |
| [POST](#Request-6)    | [/{username}/notes](#Request-6)              | Add new notes. |
| [PUT](#Request-7)     | [/{username}/notes](#Request-7)              | Edit existing notes. |
| [DELETE](#Request-8)  | [/{username}/notes/{noteId}](#Request-8)     | Delete existing note. |

### Request

```console
POST localhost:9066/user/signUp
```
Create a user with username and password. <br>

Add in authorization header Basic Authentication(`username` and `password`).

### Response
204 No Content <br>
400 Bad Request <br>
200 OK <br>

Returns access token in authorization header.

---

### Request

```console
POST localhost:9066/user/login
```
Sign in a user. <br>

Add in authorization header Basic Authentication(`username` and `password`).

### Response
204 No Content <br>
400 Bad Request <br>
404 Not Found <br>
200 OK <br>

Returns access token in authorization header.

---

### Request

```console
DELETE localhost:9066/user/{username}
```
Deletes a user. <br>

Add in authorization header Basic Authentication(`username` and `password`).

### Response
204 No Content <br>
400 Bad Request <br>
404 Not Found <br>
200 OK <br>

---

### Request

```console
GET localhost:9066/api/v1/{username}/notes
```
Lists all notes created by the user.

Add in authorization header access token(JWT). <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

### Response
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns array of note structures of type text/json. <br>
```json
[
   {
      "noteId": "string",
      "title": "string",
      "text": "string",
      "color": "string"
   }
]
```

---

### Request

```console
GET localhost:9066/api/v1/{username}/notes/{noteId}
```
List information for a note. <br>

Add in authorization header access token(JWT). <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>
_String `noteId` - Identifiers that notes must have to match the filter._ <br>

### Response
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns note structure of type text/json. <br>
```json
{
   "noteId": "string",
   "title": "string",
   "text": "string",
   "color": "string"
}
```

---

### Request

```console
GET localhost:9066/api/v1/{username}/notes?title={string}&text={string}&color={string}
```
Search for notes: _by **title**_ and/or _by **text**_ and/or _by **color**. <br>

Add in authorization header access token(JWT). <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

Query params:<br>
_String `title` - Filter notes containing this title. This field is optional._<br>
_String `text`  - Filter notes containing this text sample. This field is optional._<br>
_String `color` - Filter notes by color. This field is optional._<br>

### Response
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of filtered note structures of type text/json. <br>
```json
[
   {
      "noteId": "string",
      "title": "string",
      "text": "string",
      "color": "string"
   }
]
```

--- 

### Request

```console
POST localhost:9066/api/v1/{username}/notes
```
Add/Create new notes. <br>

Add in authorization header access token(JWT). <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

Request body: <br>
_String `title` - Note's title name._<br>
_String `text`  - Note's text content._<br>
_Array  `color` - Note's color. Used to group notes by priority. Valid colors are "yellow", "green", "red".  Default color is "yellow"._<br>
```json
[
   {
      "title": "string",
      "text": "string",
      "color": "string"
   }
]
```

### Response
204 No Content <br>
400 Bad Request <br>
201 Created <br>
Returns array of note identifiers of type text/json. <br>

```json
{
   "noteId": [
      "string"
   ]
}
```

---

### Request

```console
PUT localhost:9066/api/v1/{username}/notes
```
Edit existing notes. <br>

Add in authorization header access token(JWT). <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

Request body:<br>
_String `noteId` - Note identifier._<br>
_String `title`  - Note's title name. This field is optional._<br>
_String `text`   - Note's text content. This field is optional._<br>
_Array  `color`  - Note's color. Used to group notes by priority. Valid colors are "yellow", "green", "red".  Default color is "yellow". This field is optional._<br>

```json
[
   {
      "noteId": "string",
      "title": "string",
      "text": "string",
      "color": "string"
   }
]
```

### Response
400 Bad Request <br>
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---

### Request

```console
DELETE localhost:9066/api/v1/{username}/notes/{noteId}
```
Delete existing note. <br>

Add in authorization header access token(JWT). <br>

Path parameters:<br>
_String `username` - The user to whom we are allowing operations on notes._ <br>
_String `noteId` - Identifiers that notes must have to match the filter._<br>

### Response
400 Bad Request <br>
404 Not Found <br>
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---
