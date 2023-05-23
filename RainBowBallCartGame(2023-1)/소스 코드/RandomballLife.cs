using JetBrains.Annotations;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RandomballLife : MonoBehaviour
{
    public float StartTimeB = 1.0f;
    public float MakeRateB = 2.0f;
    public GameObject Ball;
    public static Color[] colors = { Color.red, new Color(1.0f, 0.5f, 0.0f),
        Color.yellow, Color.green, Color.blue, new Color32(75, 0, 130, 255),
        new Color32(148, 0, 211, 255) };
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("MakeRandomball", StartTimeB, MakeRateB);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    void MakeRandomball()
    {
        float Xposition = Random.Range(-10.0f, 10.0f);
        float Yposition = Random.Range(10.0f, 40.0f);
        float Zposition = Random.Range(-10.0f, 10.0f);
        int CartColor = Random.Range(0, 7);

        Vector3 position;
        position.x = Xposition; position.y = Yposition; position.z = Zposition;
        GameObject NowBall = Instantiate(Ball, position,transform.rotation);
        Renderer Rend = NowBall.GetComponent<Renderer>();
        Rend.material.color = colors[CartColor];
    }
}
