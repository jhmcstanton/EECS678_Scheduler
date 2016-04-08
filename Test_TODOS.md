# Queue

Test ideas based on priqueue_* API

## priqueue_size

### Simple cases

- No elements 
- One element 
- More than one element

### Removal Cases

- No elements, remove one
- One element, remove one
- Multiple elements, remove one
- Multiple elements, remove multiple
- Multiple elements, remove all

### Ordered Cases

...

## priqueue_remove

### Simple Cases

- No elements (expect size 0, remove 0)
- Single element remove 
    - Remove the element (expect size 1, remove 0)
    - Remove invalid element (expect size 0, remove 1)
- Multiple elements 
    - All elements equal 
        - Remove with equal element (expect size 0, remove N)
        - Remove without equal elem (expect size N, remove 0)
    - All elements not equal (monotonically increasing)
        - Remove element out of range (expect size N, remove 0)
        - Remove element in range (expect size N - 1, remove 1)
    - Repeating elements (modulo insert: i % d)
        - Remove element out of range (expect size N, remove 0)
        - Remove element in range (expect size N - N / d, remove N / d)

### Ordered Cases 

...

## priqueue_remove_at

### Simple Cases

- Out of Range (expect size N, elem NULL)
- One element (expect size 0, elem correct)
- Multiple elements (expect size N - 1, elem correct)

### Ordering cases

...


## priqueue_at

### Simple Cases

- Out of Range (expect size N, NULL)
- In Range (expect size N, elem correct)

## priqueue_poll

- ~No elements (expect size 0, elem NULL)~
- ~One element (expect size 0, elem correct)~
- ~Multiple elements (expects size N - 1, elem correct)~

## priqueue_peek

- ~No elements (expect size 0, elem NULL)~
- ~One or more elements (expect size N, elem correct)~

# Scheduler

???