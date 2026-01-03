# Welcome to BrickBins!

This program was developed to help me organize and keep track of LEGO kits I've owned over the course of my lifetime. It utilizes C++ and the SQLite3 libraries to create and update a database of sets.

### Current Commands & Usage
1. **CREATE**: Creates a database with the name following the command.

```.\BrickBins.exe CREATE [Database Name]```

2. **INSERT**: Inserts a new set into the specified database using the paramaters provided.

```.\BrickBins.exe INSERT [db] [ID] [Name] [Theme] [Pieces] [Year] [Status]```

3. **LIST**: Lists all sets currently in a specified database.

```.\BrickBins.exe LIST [db]```

4. **DELETE**: Deletes an entry from a specified database via ID.

```.\BrickBins.exe DELETE [db] [ID]```

5. **UPDATE**: Update any field of an entry in a specified database.

```.\BrickBins.exe UPDATE [db] [ID] [FIELD] [VALUE]```
