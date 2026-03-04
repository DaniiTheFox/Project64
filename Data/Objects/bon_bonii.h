#define BONI_RUNNING_STATE 0x01
#define BONI_STANDING_STATE 0x02
#define BONI_SHORT_JMP_STATE 0x03

class Bonii {
    public:
        // ----- VECTOR FOR MATH AND PHYS ----
        Vec3 boni = {0,0,0};
        float charYaw = 0.0f;
        float yawRad = 0.0f;
        float yawGl = 0.0f;
        // -----------------------------------
        float i_velocityY = 0.0f;
        float f_velocityY = -1900.0f;
        bool  is_jumping = false;
        float jumpDuration = 0.025f;
        float jumpTimer = 0.0f;
        bool  is_falling = true;
        // ----- Movement Flags --------------
        int KeyW = 0;
        int KeyA = 0;
        int KeyS = 0;
        int KeyD = 0;
        // -----------------------------------
        float moveSpeed = 5.0f;
        // -----------------------------------
        float forwardX = 0.0f;
        float forwardZ = 0.0f;
        float rightX = 0.0f;
        float rightZ = 0.0f;
        // ------- ANIMATION STATES --------
        int anim_st = BONI_STANDING_STATE;
        // ----------------------------------
        Vec3 moveDir = {0, 0, 0};
        float moveLength = 0.0f;
        // -----------------------------------
        Bonii (int,int,int);
        void Bon_Bonii_Update();
        void Bon_Bonii_Animate();
        void Bon_Bonii_Render();
        void Bon_Bonii_Gravity(float);
        void Bon_Bonii_Jump();
};

Bonii::Bonii (int _x, int _y, int _z) {
    boni.x = _x;
    boni.y = _y;
    boni.z = _z;
}

void Bonii::Bon_Bonii_Animate () {
    switch (anim_st) {
        case BONI_STANDING_STATE:
            a_bonii->setAnimationSpeed(1.0f);
            a_bonii->setAnimationRange(2, 6, true); 
        break;
        case BONI_RUNNING_STATE:
            a_bonii->setAnimationSpeed(10.0f);
            a_bonii->setAnimationRange(6, 11, true); 
        break;
        default:
            a_bonii->setAnimationSpeed(1.0f);
            a_bonii->setAnimationRange(2, 6, true);
        break;
    }
}

void Bonii::Bon_Bonii_Gravity (float deltatime) {
    float c_gravity = GRAVITY;

    i_velocityY += c_gravity * deltatime;

    if (i_velocityY < f_velocityY) {
        i_velocityY = f_velocityY;
    }

    float newY = boni.y + i_velocityY * deltatime;

    if (boni.y <= 0) {
        boni.y = (float) floor(newY) + 1;
        i_velocityY = 0;
        is_falling = false;
    } else {
        boni.y = newY;
        is_falling = true;
    }

    if (is_jumping) {
        jumpTimer += deltatime;
        if (jumpTimer >= jumpDuration) {
            is_jumping = false;
            f_velocityY = -1900.0f;
        }
    }
}

void Bonii::Bon_Bonii_Jump () {
    if (!is_jumping && !is_falling) {
        is_jumping = true;
        jumpTimer = 0;

        f_velocityY = -(f_velocityY / 3);
    }
}

void Bonii::Bon_Bonii_Update () {
    moveSpeed = 5.0f * deltaTime * 60.0f;
    float rotationSpeed = 5.0f * deltaTime * 60.0f;

    Bon_Bonii_Gravity (deltaTime);

    moveDir = {0, 0, 0};

    Bon_Bonii_Animate();
    
    if (KeyW) {
        anim_st = BONI_RUNNING_STATE;
        moveDir.x += forwardX;
        moveDir.z += forwardZ;
        isMoving = 1;
    }
    if (KeyS) {
        anim_st = BONI_RUNNING_STATE;
        moveDir.x -= forwardX;
        moveDir.z -= forwardZ;
        isMoving = 1;
    }
    if (KeyA) {
        anim_st = BONI_RUNNING_STATE;
        moveDir.x += rightX;
        moveDir.z += rightZ;
        isMoving = 1;
    }
    if (KeyD) {
        anim_st = BONI_RUNNING_STATE;
        moveDir.x -= rightX;
        moveDir.z -= rightZ;
        isMoving = 1;
    }

    if (moveDir.x == 0 || moveDir.z == 0) {
        anim_st = BONI_STANDING_STATE;
    }

    moveLength = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
    if (moveLength > 0) {
        moveDir.x /= moveLength;
        moveDir.z /= moveLength;
        
        
        boni.x += moveDir.x * moveSpeed;
        boni.z += moveDir.z * moveSpeed;
        
        if (isMoving) {
            yawGl = atan2f(moveDir.x, moveDir.z) * 180.0f / M_PI;
        }
    }

    charYaw = smoothAngle(charYaw, yawGl, rotationSpeed);
    
}

void Bonii::Bon_Bonii_Render () {
    glPushMatrix();
        glTranslatef(boni.x, boni.y+40, boni.z);
        glRotatef(charYaw, 0, 1, 0);
        a_bonii->render();
    glPopMatrix();
}

Bonii *bonii = new Bonii(0,80,0);