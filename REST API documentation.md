# Sticky-notes REST API documentation

This is the Sticky-notes REST API Reference. It provides API documentation, request/response samples, and usage description.

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

> The user must be able to
> - create notes with title, text and tags
> - edit/delete notes 
> - search for Notes: by title / by text / by tag

## REST API

| Method                | Endpoint                            | Description  |                             
| :------:              | ------                              | ------       |                               
| [GET](#Request)       | [/notes](#Request)                  | Lists all notes created by the user. |     
| [GET](#Request-1)     | [/notes/{noteId}](#Request-1)       | List information for a note. |
| [GET](#Request-2)     | [/notes?title={string}&text={string}&color={string}](#Request-2) | Filter notes: _by title_ and/or _by text_ and/or _by color_. |
| [POST](#Request-3)    | [/notes](#Request-3)                | Add new notes. |
| [PUT](#Request-4)     | [/notes](#Request-4)                | Edit existing notes. |
| [DELETE](#Request-5)  | [/notes/{noteId}](#Request-5)       | Delete existing note. |



### Request

```console
GET localhost:9066/api/v1/notes
```
Lists all notes created by the user.

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
GET localhost:9066/api/v1/notes/{noteId}
```
List information for a note. <br>

Path parameters: <br>
_String `noteId` - Identifiers that notes must have to match the filter._

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
GET /notes?title={string}&text={string}&color={string}
```
Search for notes: _by **title**_ and/or _by **text**_ and/or _by **color**. <br>

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
POST localhost:9066/api/v1/notes
```
Add/Create new notes. <br>

Request body: <br>
_String `title` - Note's title name._<br>
_String `text`  - Note's text content.This field can be empty_<br>
_Array  `color` - Note's color. Used to group notes by priority. Valid colors are yellow", "green", "red".  Default color is yellow._<br>
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
PUT localhost:9066/api/v1/notes
```
Edit existing notes. <br>

Request body:<br>
_String `noteId` - Note identifier._<br>
_String `title`  - Note's title name. This field is optional._<br>
_String `text`   - Note's text content. This field is optional._<br>
_Array  `color`  - Note's color. Used to group notes by priority. Valid colors are yellow", "green", "red".  Default color is yellow. This field is optional._<br>

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
DELETE localhost:9066/api/v1/notes/{noteId}
```
Delete existing note. <br>

Path param:<br>
_String `noteId` - Identifiers that notes must have to match the filter._<br>

### Response
400 Bad Request <br>
404 Not Found <br>
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---
