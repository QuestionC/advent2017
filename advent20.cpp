#include "advent.hpp"
#include <list>
#include <algorithm>

struct Vector3 {
    int x;
    int y;
    int z;
};

Vector3 const & operator+= (Vector3 & A, Vector3 const & B) {
    A.x += B.x;
    A.y += B.y;
    A.z += B.z;
    return A;
}

struct Particle {
    Vector3 pos;
    Vector3 dir;
    Vector3 accel;
    int id;

    Particle(int _id, char const * S) : id(_id) {
        int n = sscanf(S, "p=<%d,%d,%d>, v=<%d,%d,%d>, a=<%d,%d,%d>", 
                &pos.x, &pos.y, &pos.z,
                &dir.x, &dir.y, &dir.z,
                &accel.x, &accel.y, &accel.z
              );
        if (n != 9) {
            char buff[200];
            sprintf(buff, "Particle constructor %s doesn't have 9 arguments.", S);
            throw std::runtime_error(buff);
        }
    }

    void distance(int & accel, int & dir, int & pos) const {
        // Acceleration distance is just how fast it accelerates from 0
        accel = abs(this->accel.x) + abs(this->accel.y) + abs(this->accel.z);
    
        dir = 0;
        if (this->accel.x > 0) {
            dir += this->dir.x;
        } else if (this->accel.x < 0) {
            dir -= this->dir.x;
        } else if (this->accel.x == 0) {
            dir += abs(this->dir.x);
        }
        
        if (this->accel.y > 0) {
            dir += this->dir.y;
        } else if (this->accel.y < 0) {
            dir -= this->dir.y;
        } else if (this->accel.y == 0) {
            dir += abs(this->dir.y);
        }
        
        if (this->accel.z > 0) {
            dir += this->dir.z;
        } else if (this->accel.z < 0) {
            dir -= this->dir.z;
        } else if (this->accel.z == 0) {
            dir += abs(this->dir.z);
        }
        
        pos = 0;
        if (this->accel.x > 0) {
            pos += this->pos.x;
        } else if (this->accel.x < 0) {
            pos -= this->pos.x;
        } else if (this->accel.x == 0) {
            pos += abs(this->pos.x);
        }
        
        if (this->accel.y > 0) {
            pos += this->pos.y;
        } else if (this->accel.y < 0) {
            pos -= this->pos.y;
        } else if (this->accel.y == 0) {
            pos += abs(this->pos.y);
        }
        
        if (this->accel.z > 0) {
            pos += this->pos.z;
        } else if (this->accel.z < 0) {
            pos -= this->pos.z;
        } else if (this->accel.z == 0) {
            pos += abs(this->pos.z);
        }
    }

    bool escape(void) const {
        return
            (
             accel.x >= 0 && dir.x >= 0 && pos.x >= 0 ||
             accel.x <= 0 && dir.x <= 0 && pos.x <= 0
            ) && 
            (
             accel.y >= 0 && dir.y >= 0 && pos.y >= 0 ||
             accel.y <= 0 && dir.y <= 0 && pos.y <= 0
            ) &&
            (
             accel.z >= 0 && dir.z >= 0 && pos.z >= 0 ||
             accel.z <= 0 && dir.z <= 0 && pos.z <= 0
            );
    }

    void step(void) {
        dir += accel;
        pos += dir;
    }
};

bool collide (Particle const & A, Particle const & B) {
    bool result = A.pos.x == B.pos.x && A.pos.y == B.pos.y && A.pos.z == B.pos.z;
    return result;
}

void remove_collisions(std::vector<Particle> & PL) {
    for (auto begin = PL.begin(); begin != PL.end(); /* Iterates in loop */ ) {
        auto next = begin;
        ++next;

        auto end = std::remove_if(next, PL.end(), [begin](auto const & A) { return collide(*begin, A); });

        // Loop iteration
        if (end != PL.end()) {
            PL.erase(end, PL.end());
            begin = PL.erase(begin);
        } else {
            ++begin;
        }
    }
}

void print(FILE * f, Particle const & P) {
    printf("%d: p=(%d, %d, %d), v=(%d, %d, %d), a=(%d,%d,%d)",
            P.id,
            P.pos.x, P.pos.y, P.pos.z,
            P.dir.x, P.dir.y, P.dir.z,
            P.accel.x, P.accel.y, P.accel.z);
}


int main (void) {
    FILE * f = fopen("input20.txt", "r");
    char buff[255];
    char * p = buff;
    size_t line_length = 255;

    std::vector<Particle> particles;

    int id = 0;
    while (getline(&p, &line_length, f) > 0) {
        if (line_length != 255) {
            printf("Line longer than expected %s\n", p);
            exit(-1);
        }

        particles.push_back(Particle(id++, p));
    }

    for (auto i = particles.begin(); i != particles.end(); ++i) {
        print(stdout, *i);
        printf("\n");
    }

    // Find the particle which is closest to origin.
    // Minimize on accel

    std::list<Particle> min_particles;
    int min_accel= 999999;
    int min_vel = 0;
    int min_pos = 0;
    for (auto i = particles.begin(); i != particles.end(); ++i) {
        int A, V, P;
        i->distance(A, V, P);
        if (
                A < min_accel ||
                A == min_accel && V < min_vel ||
                A == min_accel && V == min_vel && P < min_pos) {
            min_accel = A;
            min_vel = V;
            min_pos = P;

            min_particles.clear();
            min_particles.push_back(*i);
        } else if (A == min_accel && V == min_vel && P == min_pos) {
            min_particles.push_back(*i);
        }
    }

    for (auto i = min_particles.begin(); i != min_particles.end(); ++i) {
        print(stdout, *i);
        printf("\n");
    }

    Particle P(69, "p=<138,-36,2926>, v=<16,-7,422>, a=<-2,4,-29>");
    print(stdout, P);
    print(stdout, '\n');
    P.step();
    print(stdout, P);
    print(stdout, '\n');

    std::vector<Particle> PL;
    PL.push_back(Particle(1, "p=<138,-36,2926>, v=<16,-7,422>, a=<-2,4,-29>"));
    PL.push_back(Particle(2, "p=<138,-36,2926>, v=<16,-7,423>, a=<-2,4,-29>"));
    PL.push_back(Particle(3, "p=<138,-36,2927>, v=<16,-7,424>, a=<-2,4,-29>"));

    print(stdout, PL);
    print(stdout, '\n');

    remove_collisions(PL);

    print(stdout, PL);
    print(stdout, '\n');

    for (int time = 1; time < 100'000; ++time) {
        // Remove any colliding particles
        remove_collisions(particles);

        // Step each particle
        std::for_each(particles.begin(), particles.end(), [](Particle & P){ P.step(); });

        // Progress meter
        if (time % 1'000 == 0) {
            printf ("Time: %d, Particles: %d\n", time, particles.size());
        }
    }

    printf ("Ended with %d particles\n", particles.size());

    return 0;
}
