# Sticky-notes REST API documentation

This is the Sticky-notes REST API Reference. It provides API documentation, request/response samples, and usage description.

Based on [Define Note's REST API task](https://github.com/aivaraleksiev/Sticky-notes.com/issues/4)

> The user must be able to
> - create notes with title, text and tags
> - edit/delete notes 
> - create/edit/delete tags
> - search for Notes: by title / by text / by tag

## REST API

| Method                | Endpoint                            | Description  |                             
| :------:              | ------                              | ------       |                               
| [GET](#Request)       | [/notes](#Request)                  | Lists all notes created by the user. |     
| [GET](#Request-1)     | [/notes/{noteId}](#Request-1)       | List information for a note. |
| [GET](#Request-2)     | [/notes/tags](#Request-2)           | Lists all tags created by the user and the asscoicated notes with them. |
| [GET](#Request-3)     | [/notes/tags/{tagId}](#Request-3)   | List information for a tag. |
| [GET](#Request-4)     | [/notes?title={string}&text={string}&tag={string}](#Request-4) | Filter notes: _by title_ and/or _by text_ and/or _by tag._ |
| [POST](#Request-5)    | [/notes](#Request-5)                | Add/Create new notes. |
| [PUT](#Request-6)     | [/notes](#Request-6)                | Edit existing notes. |
| [POST](#Request-7)    | [/notes/tags](#Request-7)           | Add new tags.  |
| [PUT](#Request-8)     | [/notes/tags/](#Request-8)          | Edit existing tags. |
| [DELETE](#Request-9)  | [/notes/{noteId}](#Request-9)       | Delete existing note. |
| [DELETE](#Request-10) | [/notes/tags/{tagId}](#Request-10)  | Delete existing tags.  |



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
      "tags": [
         {
            "tagId": "string",
            "name": "string"
         }
      ]
   }
]
```

---

### Request

```console
GET localhost:9066/api/v1/notes/{noteId}
```
List information for a note. <br><br>

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
   "tags": [
      {
         "tagId": "string",
         "name": "string",
         "associations": [
            "string"
         ]
      }
   ]
}
```

---

### Request

```console
GET localhost:9066/api/v1/notes/tags
```
Lists all tags created by the user and the asscoicated notes with them.


### Response
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns array of tag structures of type text/json. <br>
```json
[
   {
      "tagId": "string",
      "name": "string",
      "associations": [
         "string"
      ]
   }
]
```

---

### Request

```console
GET localhost:9066/api/v1/notes/tags/{tagId}
```
List information for a tag.<br>

Path parameters: <br>
_String `noteId` - Identifiers that tags must have to match the filter._

### Response
204 No Content <br>
400 Bad Request <br>
200 OK <br>
Returns tag structure of type text/json. <br>
```json
{
   "tagId": "string",
   "name": "string",
   "associations": [
      "string"
   ]
}
```

--- 

### Request

```console
GET /notes?title={string}&text={string}&tag={string}
```
Search for notes: _by **title**_ and/or _by **text**_ and/or _by **tagName**. <br>

Query params:<br>
_String `title` - Filter notes containing this title. This field is optional._<br>
_String `text`  - Filter notes containing this text sample. This field is optional._<br>
_String `tag`   - Filter notes associated with this tag name. This field is optional._<br>

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
      "tags": [
         {
            "tagId": "string",
            "name": "string",
            "associations": [
               "string"
            ]
         }
      ]
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
_Array  `tags`  - Tags attached to this note. Must provide existing tag identifiers. This field can be empty._<br>
```json
[
   {
      "title": "string",
      "text": "string",
      "tags": [
	     "string"
        ]
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
_String `title`  - Note's title name. This field if optional._<br>
_String `text`   - Note's text content. This field if optional._<br>
_Array  `tagId`  - Tags attached to this note. Must provide existing tag identifiers. This field if optional._<br>

```json
[
   {
      "noteId": "string",
      "title": "string",
      "text": "string",
      "tagId": [
         "string"
      ]
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
POST localhost:9066/api/v1/notes/tags
```
Add new tags. <br>

Request body:<br>
_String `name`        - Tag name. Must be unique._<br>
_Array `associations` - Consists of note identifiers to which this tag is attached. This field can be empty._<br>


```json
[
   {   
      "name": "string",
      "associations": [
         "string"
      ]
   }
]
```

### Response
400 Bad Request <br>
403 Already exists. <br>
201 Created <br>
Returns array of tag identifiers of type text/json. <br>

```json
{
   "tagId": [
      "string"
   ]
}
```

---

### Request

```console
PUT localhost:9066/api/v1/notes/tags
```
Edit existing tags. <br>

Request body:<br>
_String `tagId`       - The tag identifier._<br>
_String `name`        - Tag name. Must be unique. This field is optional._<br>
_Array `associations` - Consists of notes to which this tag is attached. Must provide existing note identifiers. This field is optional._<br>

```json
[
   {
      "tagId": "string",
      "name": "string",
      "associations": [
         "string"
      ]
   }
]
```

### Response
400 Bad Request <br>
403 Already exists. <br>
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

### Request

```console
DELETE localhsot:9066/api/v1/notes/tags/{tagId}
```
Delete existing tag. <br>

Path param: <br>
_String `tagId` - Identifiers that tags must have to match the filter._<br>

### Response
400 Bad Request <br>
404 Not Found <br>
204 No Content <br>
Operation does not return any data structure. Updated successfully. <br> 

---