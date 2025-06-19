using UnityEngine;

public class MakeHammer : MonoBehaviour
{
    public GameObject hammer;
    private GameObject axisX;
    private GameObject axisZ;


    void Start()
    {
        axisX = GameObject.Find("Point");
        axisZ = GameObject.Find("Point");
    }


    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            makeHammer();
        }
    }

    void makeHammer()
    {
        float xpos = axisX.transform.position.x;
        float zpos = axisZ.transform.position.z;
        Vector3 pos = transform.position;
        pos.x = xpos;
        pos.z = zpos - 1;
        Instantiate(hammer, pos, hammer.transform.rotation);

    }
}
