using UnityEngine;

public class MoveMole : MonoBehaviour
{
    public float ymax = 1.5f;
    public float ymin = -1.5f;
    public float speed = 1.0f;
    private float ypos = 0.0f;

    
    void Start()
    {
        ypos = ymin;
    }

   
    void Update()
    {
        ypos += speed * Time.deltaTime;
        if (ypos > ymax)
        {
            speed = -speed;
        }
        else if (ypos < ymin)
        {
            Destroy(gameObject);
        }
        else
        {
            Vector3 pos = transform.position;
            pos.y = ypos;
            transform.position = pos;
        }
    }
}
