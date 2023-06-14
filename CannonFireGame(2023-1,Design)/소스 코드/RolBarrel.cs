using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;

public class RolBarrel : MonoBehaviour
{
    public GameObject Blush;
    public GameObject Bullet;
    float rotSpeed = 100.0f;
    public float power = 400;
    public AudioSource audioSource;
    public AudioClip clipgunfire;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float zoff = Input.GetAxis("Vertical") * rotSpeed * Time.deltaTime;
        transform.Rotate(0f,0f,zoff);
        Vector3 rotation = transform.rotation.eulerAngles;
        float zang = rotation.z;
        if (zang < 0.0f || zang > 150.0f) transform.Rotate(0f,0f,-zoff);

        if (Input.GetKeyDown(KeyCode.Space))
        {
            fireBullet();
        }
        
    }
    
    void fireBullet()
    {
        Vector3 position = transform.position + transform.right * 2.0f + transform.up * 0.4f;
        GameObject newBullet = Instantiate(Bullet, position, transform.rotation);
        Rigidbody2D rb = newBullet.GetComponent<Rigidbody2D>();
        rb.AddForce(power * (transform.right * 2.0f + transform.up * 1.0f));
        Instantiate(Blush, position, transform.rotation);
        gunfire();
    }

    void gunfire()
    {
        audioSource.volume = 1.0f;
        audioSource.PlayOneShot(clipgunfire);
    }
}
