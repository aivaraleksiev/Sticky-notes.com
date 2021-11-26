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
| [POST](#Request)      | [/user/signUp](#Request)                     | Creates user with username and password. |
| [POST](#Request-1)    | [/user/login](#Request-1)                    | Sign in user. |
| [POST](#Request-2)    | [/{username}/passwordChange](#Request-2)     | Change user password.|
| [DELETE](#Request-3)  | [/user/{username}](#Request-3)               | Deletes a user. |       
| [GET](#Request-4)     | [/{username}/notes](#Request-4)              | Lists all notes created by the user. |     
| [GET](#Request-5)     | [/{username}/notes/{noteId}](#Request-5)     | List information for a note. |
| [GET](#Request-6)     | [/{username}/notes?title={string}&text={string}&color={string}](#Request-6) | Filter notes: _by title_ and/or _by text_ and/or _by color_. |
| [POST](#Request-7)    | [/{username}/notes](#Request-7)              | Add new notes. |
| [PUT](#Request-8)     | [/{username}/notes](#Request-8)              | Edit existing notes. |
| [DELETE](#Request-9)  | [/{username}/notes/{noteId}](#Request-9)     | Delete existing note. |

### Request

```console
POST https://localhost:9066/api/v1/user/signUp
```
Create a user with username and password. <br>
Username/password must not be empty and longer must not be than 50 characters.<br><br>

_Authorization header_ <br>
Add Basic Authentication(`username` and `password`).

### Response
500 Internal Server Error <br>
400 Bad Request <br>
204 No Content <br>

Returns `access token(JWT)` in authorization header.

---

### Request

```console
POST https://localhost:9066/api/v1/user/login
```
Sign in a user. <br><br>

_Authorization header_ <br>
Add Basic Authentication(`username` and `password`).

### Response
500 Internal Server Error <br>
400 Bad Request <br>
401 Unauthorized  <br>
204 No Content <br>

Returns `access token(JWT)` in authorization header.

---

### Request

```console
POST https://localhost:9066/api/v1/{username}/passswordChange
```
Change user password. <br><br>

Request body:<br>
_String `oldPassword` - The currently used password._<br>
_String `newPassword` - The new password._<br>

```json
[
   {
      "oldPassword": "string",
      "newPassword": "string",
   }
]
```

### Response
500 Internal Server Error <br>
400 Bad Request <br>
401 Unauthorized  <br>
204 No Content <br>

---

### Request

```console
DELETE https://localhost:9066/api/v1/user/{username}
```
Deletes a user. <br><br>

_Authorization header_<br>
Add Basic Authentication(`username` and `password`).<br>

### Response
204 No Content <br>
400 Bad Request <br>
404 Not Found <br>

---

### Request

```console
GET https://localhost:9066/api/v1/{username}/notes
```
Lists all notes created by the user.<br><br>

_Authorization header_ <br>
Add `access token(JWT)`. <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

### Response
500 Internal Server Error <br>
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns array of note structures of type text/json. <br>
```json
[
   {
      "noteId": "integer",
      "title": "string",
      "text": "string",
      "color": "string" // red|yellow|green
   }
]
```

---

### Request

```console
GET https://localhost:9066/api/v1/{username}/notes/{noteId}
```
List information for a note. <br><br>

_Authorization header_ <br>
Add `access token(JWT)`. <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>
_String `noteId` - Identifiers that notes must have to match the filter._ <br>

### Response
500 Internal Server Error <br>
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns note structure of type text/json. <br>
```json
{
   "noteId": "integer",
   "title": "string",
   "text": "string",
   "color": "string" // red|yellow|green
}
```

---

### Request

```console
GET https://localhost:9066/api/v1/{username}/notes?title={string}&text={string}&color="red"|"yellow"|"green"
```
Search for notes: _by **title**_ and/or _by **text**_ and/or _by **color**. <br><br>

_Authorization header_ <br>
Add `access token(JWT)`. <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

Query params:<br>
_String `title` - Filter notes containing this title. This field is optional._<br>
_String `text`  - Filter notes containing this text sample. This field is optional._<br>
_String `color` - Filter notes by color. Used to group notes by priority. Valid colors are "yellow", "green", "red". This field is optional._<br>

### Response
500 Internal Server Error <br>
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns Array of filtered note structures of type text/json. <br>
```json
[
   {
      "noteId": "integer",
      "title": "string",
      "text": "string",
      "color": "string" // red|yellow|green
   }
]
```

--- 

### Request

```console
POST https://localhost:9066/api/v1/{username}/notes
```
Add/Create new notes. <br><br>

_Authorization header_ <br>
Add `access token(JWT)`. <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

Request body: <br>
_String `title` - Note's title name._<br>
_String `text`  - Note's text content._<br>
_String `color` - Note's color. Used to group notes by priority. Valid colors are "yellow", "green", "red". If the color is invalid, it will be set to "yellow"._<br>
```json
[
   {
      "title": "string",
      "text": "string",
      "color": "string" // red|yellow|green
   }
]
```

### Response
500 Internal Server Error <br>
204 No Content <br>
400 Bad Request <br>
201 Created <br>
Returns array of note identifiers of type text/json. <br>

```json
{
   "noteId": [ "integer" ]
}
```

---

### Request

```console
PUT https://localhost:9066/api/v1/{username}/notes
```
Edit existing notes. <br><br>

_Authorization header_ <br>
Add `access token(JWT)`. <br>

Path parameters: <br>
_String `username` - The user to whom we are allowing operations on notes._ <br>

Request body:<br>
_Integer `noteId` - Note identifier._<br>
_String `title`  - Note's title name. This field is optional._<br>
_String `text`   - Note's text content. This field is optional._<br>
_String `color` - Note's color. Used to group notes by priority. Valid colors are "yellow", "green", "red". This field is optional._<br>

```json
[
   {
      "noteId": "integer",
      "title": "string",
      "text": "string",
      "color": "string" // red|yellow|green
   }
]
```

### Response
500 Internal Server Error <br>
400 Bad Request <br>
```json
[
    {
        "Http error code": "integer",
        "noteId": "integer",
        "reason": "string"
    }
]
```
404 Not Found <br>
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---

### Request

```console
DELETE https://localhost:9066/api/v1/{username}/notes/{noteId}
```
Delete existing note. <br><br>

_Authorization header_ <br> 
Add `access token(JWT)`. <br>

Path parameters:<br>
_String `username` - The user to whom we are allowing operations on notes._ <br>
_String `noteId` - Identifiers that notes must have to match the filter._<br>

### Response
500 Internal Server Error <br>
400 Bad Request <br>
404 Not Found <br>
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---
