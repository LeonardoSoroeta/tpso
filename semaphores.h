int binary_semaphore_allocation (key_t key, int sem_flags);

/* Deallocate a binary semaphore. All users must have finished their
use. Returns -1 on failure. */
int binary_semaphore_deallocate (int semid);

int binary_semaphore_initialize (int semid);

int binary_semaphore_wait (int semid);

/* Post to a binary semaphore: increment its value by 1.
This returns immediately. */
int binary_semaphore_post (int semid);