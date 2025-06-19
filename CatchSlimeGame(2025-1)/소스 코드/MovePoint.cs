using UnityEngine;

public class MovePoint : MonoBehaviour
{
    public float xmin = -8.0f;
    public float xmax = 8.0f;
    private float speed = 0.0f;
    public float zmin = -8.0f;
    public float zmax = 8.0f;

    void Start()
    {
        GameManager gameManager = FindAnyObjectByType<GameManager>();
        speed = gameManager.axisSpeed;
    }


    void Update()
    {
        float xoff = Input.GetAxis("Horizontal") * speed * Time.deltaTime;
        float xpos = transform.position.x + xoff; // ´ÙÀ½¹ø x ÁÂÇ¥
        if (xpos >= xmin && xpos <= xmax)
            transform.Translate(xoff, 0.0f, 0.0f);

        float zoff = Input.GetAxis("Vertical") * speed * Time.deltaTime;
        float zpos = transform.position.z + zoff; // ´ÙÀ½¹ø z ÁÂÇ¥
        if (zpos >= zmin && zpos <= zmax) // Assuming zmin and zmax are -10.0f and 10.0f respectively
            transform.Translate(0.0f, 0.0f, zoff);
    }
}

